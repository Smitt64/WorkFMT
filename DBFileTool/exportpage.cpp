#include "exportpage.h"
#include "ui_exportpage.h"
#include "loghighlighter.h"
#include "dbfileobject.h"
#include "dbttoolwizard.h"
#include <fmtcore.h>
#include <QDebug>
#include <QRunnable>
#include <QThreadPool>
#include <QProcess>
#include <QTemporaryDir>
#include <QMutex>
#include <limits>
#include <QPushButton>

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

    qDebug() << "Temp directory remove:" << tmpdir.remove();

    pParent->m_Complete = true;
    emit pParent->completeChanged();
}

ExportPage::ExportPage(QWidget *parent) :
    QWizardPage(parent),
    ui(new Ui::ExportPage)
{
    ui->setupUi(this);

    m_Complete = false;
    logColor = new LogHighlighter(ui->plainTextEdit->document());
    setFinalPage(true);
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

    RsExpOperation *pObj = new RsExpOperation(pWizard, this);
    connect(pObj, SIGNAL(procMessage(QString)), ui->plainTextEdit, SLOT(appendPlainText(QString)));
    QThreadPool::globalInstance()->start(pObj);
}

void ExportPage::setCompleteOn()
{
    m_Complete = true;
}

bool ExportPage::isComplete() const
{
    return m_Complete;
}
