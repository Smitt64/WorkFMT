#include "exportpage.h"
#include "selectactionpage.h"
#include "ui_exportpage.h"
#include "loghighlighter.h"
#include "dbfileobject.h"
#include "dbttoolwizard.h"
#include "exportobject.h"
#include "importobject.h"
#include "export/exporterfactory.h"
#include "connectioninfo.h"
#include <fmtcore.h>
#include <QDebug>
#include <QRunnable>
#include <QThreadPool>
#include <QProcess>
#include <QTemporaryDir>
#include <QMutex>
#include <limits>
#include <QPushButton>
#include <QFileInfo>
#include <QDir>
#include <QDateTime>

RsExpOperationOld::RsExpOperationOld(DbtToolWizard *Wizard, ExportPage *parent) :
    QObject(),
    QRunnable(),
    pParent(parent),
    pWizard(Wizard)
{
    setAutoDelete(true);
}

void RsExpOperationOld::run()
{
    QString programmPath, ociPath, oraociPath;

    QTemporaryDir tmpdir;
    QDir tmpDir(tmpdir.path());

    QProcess proc;
    QProcessEnvironment env = QProcessEnvironment::systemEnvironment();
    proc.setEnvironment(env.toStringList());
    proc.setWorkingDirectory(pWizard->field("ExportPath").toString());

    programmPath = tmpDir.absoluteFilePath("RSexp.exe");
    ociPath = tmpDir.absoluteFilePath("oci.dll");
    oraociPath = tmpDir.absoluteFilePath("oraocci11.dll");

    qDebug() << "Temp directory:" << tmpdir.path();
    qDebug() << "Copy RSexp:" << QFile::copy(":/RSexp" , programmPath);
    qDebug() << "Copy oci:" << QFile::copy(":/oci" , ociPath);
    qDebug() << "Copy oraocci11:" << QFile::copy(":/oraocci11" , oraociPath);
    qDebug() << "RSexp path:" << programmPath;

    connect(&proc, &QProcess::readyReadStandardError, [=, &proc]()
    {
        QString err = proc.readAllStandardError();
        emit procMessage(err);
        emit procError(err);
    });

    connect(&proc, &QProcess::readyReadStandardOutput, [=, &proc]()
    {
        QString msg = proc.readAllStandardOutput();
        if (msg.contains(QRegExp("ORA-\\d{5}[^\\n]*")))
            emit procError(msg.simplified());
        emit procMessage(msg);
    });

    QStringList lst;
    QVariant val = pWizard->userField("TableList");

    if (val.isValid())
        lst = val.toStringList();

    foreach (const QString &table, lst)
    {
        QStringList arguments = QStringList()
                                  << pWizard->field("User").toString()
                                  << pWizard->field("Password").toString()
                                  << pWizard->field("Service").toString()
                                  << table;

        emit procMessage("*************** Start unloading ***************");
        CoreStartProcess(&proc, tmpDir.absoluteFilePath("RSexp.exe"), arguments, true, true,
                         std::numeric_limits<int>::max());
    }

    pParent->m_Complete = true;
    emit pParent->completeChanged();
}

// ---------------------------------------------------------------------------
RsExpOperation::RsExpOperation(DbtToolWizard *Wizard, ExportPage *parent) :
    QObject(),
    QRunnable(),
    pParent(parent),
    pWizard(Wizard)
{
    setAutoDelete(true);
}

void RsExpOperation::run()
{
    QDir dir(pWizard->field("ExportPath").toString());
    QStringList lst;
    QVariant val = pWizard->userField("TableList");

    if (val.isValid())
        lst = val.toStringList();

    /*foreach (const QString &table, lst)
    {
        emit procMessage("*************** Start unloading ***************");

        QScopedPointer<ExportObject> obj(new ExportObject());
        obj->setConnectionInfo(pWizard->field("User").toString(),
                               pWizard->field("Password").toString(),
                               pWizard->field("Service").toString(), false);

        obj->setClobMode((ExportObject::ClobMode)ClobMode);
        obj->exportTable(table, dir);
    }*/
    QString options;
    QScopedPointer<ConnectionInfo> info(new ConnectionInfo());

    if (!info->open(QRSD_DRIVER, pWizard->field("User").toString(),
                    pWizard->field("Password").toString(),
                    pWizard->field("Service").toString(), options))
    {
        pParent->m_Complete = true;
        emit pParent->completeChanged();

        return;
    }

    QScopedPointer<ExporterBase> pExporter(ExporterFactory::createExporter(info.data()));

    pExporter->setConnectionInfo(info.data());
    pExporter->setClobMode((ExporterBase::ClobMode)ClobMode);
    pExporter->setOutputDirectory(dir);
    connect(pExporter.data(), &ExporterBase::procMessage, this, &RsExpOperation::procMessage);

    for (const QString &table : qAsConst(lst))
        pExporter->exportTable(table);

    pParent->m_Complete = true;
    emit pParent->completeChanged();
}
// ---------------------------------------------------------------------------
RsImpOperation::RsImpOperation(DbtToolWizard *Wizard, ExportPage *parent) :
    QObject(),
    QRunnable(),
    pParent(parent),
    pWizard(Wizard)
{
    setAutoDelete(true);
}

void RsImpOperation::run()
{
    QDir dir(pWizard->field("ExportPath").toString());
    QStringList lst;
    QVariant val = pWizard->userField("TableList");

    if (val.isValid())
        lst = val.toStringList();

    if (lst.isEmpty())
    {
        pParent->m_Complete = true;
        emit pParent->completeChanged();
        return;
    }

    struct ImportStat
    {
        QString fileName;
        int totalRows = 0;
        int failedRows = 0;
        bool success = false;
        qint64 elapsedMs = 0;
    };

    QList<ImportStat> stats;
    int totalImported = 0;
    int totalFailed = 0;
    int totalFiles = 0;
    int failedFiles = 0;

    QDateTime runStart = QDateTime::currentDateTime();

    QScopedPointer<ConnectionInfo> info(new ConnectionInfo());

    if (!info->open(QRSD_DRIVER, pWizard->field("User").toString(),
                    pWizard->field("Password").toString(),
                    pWizard->field("Service").toString(), ""))
    {
        emit procError(tr("Не удалось открыть подключение"));
        pParent->m_Complete = true;
        emit pParent->completeChanged();
        return;
    }

    if (info->type() == ConnectionInfo::CON_ORA)
    {
        // Oracle: используем sqlplus + FillTable.sql (как DBFileObject::load)
        QTemporaryDir tmpdir;
        QDir tmpDir(tmpdir.path());
        QString programmPath = tmpDir.absoluteFilePath("FillTable.sql");
        QFile::copy(":/FillTable", programmPath);

        QProcess proc;
        QProcessEnvironment env = QProcessEnvironment::systemEnvironment();
        env.insert("NLS_NUMERIC_CHARACTERS", ".,");
        env.insert("NLS_LANG", "AMERICAN_CIS.RU8PC866");
        proc.setEnvironment(env.toStringList());

        connect(&proc, &QProcess::readyReadStandardError, [&proc, this]()
        {
            QString err = proc.readAllStandardError();
            emit procError(err);
            emit procMessage(err);
        });

        connect(&proc, &QProcess::readyReadStandardOutput, [&proc, this]()
        {
            QString msg = proc.readAllStandardOutput();
            if (msg.contains(QRegExp("ORA-\\d{5}[^\\n]*")))
                emit procError(msg.simplified());
            emit procMessage(msg);
        });

        QString user = pWizard->field("User").toString();
        QString password = pWizard->field("Password").toString();
        QString service = pWizard->field("Service").toString();

        for (const QString &importFile : qAsConst(lst))
        {
            totalFiles++;
            QFileInfo fi(dir.absoluteFilePath(importFile));
            QString table = fi.baseName();
            QString workDirectory = fi.absoluteDir().path();
            ImportStat stat;
            stat.fileName = fi.fileName();

            QDateTime fileStart = QDateTime::currentDateTime();

            emit procMessage("**************** Start loading ****************");

            proc.setWorkingDirectory(workDirectory);
            proc.setProgram("sqlplus");
            proc.setArguments(QStringList()
                              << QString("%1/%2@%3").arg(user, password, service)
                              << QString("@%1").arg(programmPath)
                              << table
                              << table
                              << user
                              << password
                              << service);
            proc.start();
            proc.waitForFinished(-1);

            stat.success = (proc.exitCode() == 0 && proc.exitStatus() == QProcess::NormalExit);
            if (!stat.success)
                failedFiles++;
            stat.elapsedMs = fileStart.msecsTo(QDateTime::currentDateTime());

            stats.append(stat);
        }
    }
    else if (info->type() == ConnectionInfo::CON_POSTGRESQL)
    {
        QScopedPointer<ImportObject> importer(new ImportObject);
        importer->setConnectionInfo(pWizard->field("User").toString(),
                                     pWizard->field("Password").toString(),
                                     pWizard->field("Service").toString(),
                                     false);

        connect(importer.data(), &ImportObject::procMessage, this, &RsImpOperation::procMessage);
        connect(importer.data(), &ImportObject::procError, this, &RsImpOperation::procError);
        connect(importer.data(), &ImportObject::procInfo, this, &RsImpOperation::procInfo);

        for (const QString &importFile : qAsConst(lst))
        {
            totalFiles++;
            QFileInfo fi(dir.absoluteFilePath(importFile));
            ImportStat stat;
            stat.fileName = fi.fileName();

            QDateTime fileStart = QDateTime::currentDateTime();

            bool fileSuccess = false;
            int fileRows = 0;

            connect(importer.data(), &ImportObject::importTableFinished, [&fileSuccess, &fileRows](const QString &datFile, bool success, int rows)
            {
                Q_UNUSED(datFile)
                fileSuccess = success;
                fileRows = rows;
            });

            emit procMessage(QString("Control File:   %1").arg(fi.fileName()));

            importer->importTable(importFile, dir);

            stat.success = fileSuccess;
            stat.totalRows = fileRows;

            if (!stat.success)
                failedFiles++;

            stat.elapsedMs = fileStart.msecsTo(QDateTime::currentDateTime());
            totalImported += stat.totalRows;
            totalFailed += stat.failedRows;

            stats.append(stat);
        }
    }
    else
    {
        emit procError(tr("Неподдерживаемый тип подключения для импорта"));
    }

    QDateTime runEnd = QDateTime::currentDateTime();

    // Итоговый лог в стиле SQL*Loader.
    emit procMessage("");
    emit procMessage("Summary:");
    emit procMessage("------------------------------");

    for (const ImportStat &s : qAsConst(stats))
    {
        emit procMessage(QString("Table %1:").arg(s.fileName));
        emit procMessage(QString("  %1 Rows successfully loaded.").arg(s.totalRows));
        emit procMessage(QString("  %1 Rows not loaded due to data errors.").arg(s.failedRows));
        emit procMessage("  0 Rows not loaded because all WHEN clauses were failed.");
        emit procMessage("  0 Rows not loaded because all fields were null.");
        emit procMessage(QString("  Status: %1").arg(s.success ? tr("OK") : tr("FAILED")));
        emit procMessage(QString("  Elapsed: %1").arg(QTime::fromMSecsSinceStartOfDay(s.elapsedMs).toString("hh:mm:ss.zzz")));
        emit procMessage("");
    }

    emit procMessage(QString("Total files:      %1").arg(totalFiles));
    emit procMessage(QString("Total rows:       %1").arg(totalImported));
    emit procMessage(QString("Rows failed:      %1").arg(totalFailed));
    emit procMessage(QString("Files failed:     %1").arg(failedFiles));
    emit procMessage(QString("Run began on:     %1").arg(runStart.toString("ddd MMM dd hh:mm:ss yyyy")));
    emit procMessage(QString("Run ended on:     %1").arg(runEnd.toString("ddd MMM dd hh:mm:ss yyyy")));
    emit procMessage(QString("Elapsed time was: %1").arg(QTime::fromMSecsSinceStartOfDay(runStart.msecsTo(runEnd)).toString("hh:mm:ss.zzz")));

    pParent->m_Complete = true;
    emit pParent->completeChanged();
}
// ---------------------------------------------------------------------------

ExportPage::ExportPage(QWidget *parent) :
    QWizardPage(parent),
    ui(new Ui::ExportPage)
{
    ui->setupUi(this);

    m_Complete = false;
    logColor = new LogHighlighter(ui->plainTextEdit->document());
    setFinalPage(true);
    setTitle(tr("Экспорт данных в *.dat файл"));
}

ExportPage::~ExportPage()
{
    delete ui;
}

void ExportPage::initializePage()
{
    DbtToolWizard *pWizard = qobject_cast<DbtToolWizard*>(wizard());

    m_Complete = false;
    wizard()->button(QWizard::BackButton)->setEnabled(false);

    int action = field("Action").toInt();

    // Заголовок страницы зависит от режима (импорт/экспорт)
    if (action == SelectActionPage::ActionImportOra)
        setTitle(tr("Импорт данных из *.dat файла"));
    else
        setTitle(tr("Экспорт данных в *.dat файл"));

    if (action == SelectActionPage::ActionExportOraOld)
    {
        RsExpOperationOld *pObj = new RsExpOperationOld(pWizard, this);
        connect(pObj, SIGNAL(procMessage(QString)), ui->plainTextEdit, SLOT(appendPlainText(QString)));
        QThreadPool::globalInstance()->start(pObj);
    }
    else if (action == SelectActionPage::ActionExportOra)
    {
        RsExpOperation *pObj = new RsExpOperation(pWizard, this);
        pObj->ClobMode = field("ClobMode").toInt();

        connect(pObj, SIGNAL(procMessage(QString)), ui->plainTextEdit, SLOT(appendPlainText(QString)));
        QThreadPool::globalInstance()->start(pObj);
    }
    else if (action == SelectActionPage::ActionImportOra)
    {
        RsImpOperation *pObj = new RsImpOperation(pWizard, this);
        connect(pObj, SIGNAL(procMessage(QString)), ui->plainTextEdit, SLOT(appendPlainText(QString)));
        connect(pObj, SIGNAL(procInfo(QString)), ui->plainTextEdit, SLOT(appendPlainText(QString)));
        connect(pObj, SIGNAL(procError(QString)), ui->plainTextEdit, SLOT(appendPlainText(QString)));
        QThreadPool::globalInstance()->start(pObj);
    }
}

void ExportPage::setCompleteOn()
{
    m_Complete = true;
}

bool ExportPage::isComplete() const
{
    return m_Complete;
}
