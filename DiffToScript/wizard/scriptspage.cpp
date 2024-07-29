#include "scriptspage.h"
#include "ui_scriptspage.h"
#include <codeeditor/codeeditor.h>
#include <codeeditor/codehighlighter.h>
#include "fmtcore.h"
#include "toolsruntime.h"
#include "diffwizard.h"
#include "tablelinks.h"
#include "task.h"
#include "svnlogmodel.h"
#include "ErrorsModel.h"
#include "errordlg.h"
#include <QProcess>
#include <QThreadPool>
#include <QMapIterator>
#include <QTemporaryFile>
#include <QTextStream>
#include <QSplitter>
#include <QScrollBar>
#include <QTabWidget>

enum
{
    ActionByLocalDiff = 0
};

GenerateOperation::GenerateOperation(DiffWizard *wizrd) :
    QObject(),
    QRunnable(),
    m_Errors(nullptr)
{
    m_pWzrd = wizrd;
    setAutoDelete(true);
}

GenerateOperation::~GenerateOperation()
{

}

void GenerateOperation::setErrorsBuf(ErrorsModel *err)
{
    m_Errors = err;
}
// --delete --insert --update --cs "CONNSTRING=dsn=THOR_DB12DEV1;user id=SERP_3188;password=SERP_3188" --input diff.txt
void GenerateOperation::run()
{
    int Action = m_pWzrd->field("Action").toInt();
    QString Path = m_pWzrd->field("Path").toString();
    QString User = m_pWzrd->field("User").toString();
    QString Password = m_pWzrd->field("Password").toString();
    QString Service = m_pWzrd->field("Service").toString();
    QString Revision = m_pWzrd->field("Revision").toString();

    SvnInfoMap info = SvnGetRepoInfo(Path);

    bool IsUnicode = m_pWzrd->field("IsUnicode").toBool();
    QStringList files = m_pWzrd->userField("Files").toStringList();
    QStringList filesCleared = files;
    QList<TableLinks> tableLinks;

    for (const QString &file : files)
    {
        QFileInfo fi(file);
        QString fileName = toolFullFileNameFromDir(QString("relations/%1.json").arg(fi.baseName().toLower()));

        if (QFile::exists(fileName))
        {
            tableLinks.append(TableLinks());
            TableLinks& tableLink = tableLinks.back();
            tableLink.loadLinks(fileName);

            for (const Link &childlnk : tableLink.links)
            {
                QStringList::iterator pos = std::find_if(files.begin(), files.end(), [=](const QString &f)
                {
                    return f.contains(childlnk.tableName, Qt::CaseInsensitive);
                });

                if (pos != files.end())
                    filesCleared.removeOne(*pos);
            }

            filesCleared.removeOne(file);
        }
    }

    QMap<QString, QByteArray> DiffData;
    QStringList args = { "diff" };

    if (!Revision.isEmpty())
    {
        args.append("-c");
        args.append(Revision);
    }

    for (const TableLinks &tableLink : tableLinks)
    {
        QStringList::iterator file = std::find_if(files.begin(), files.end(), [=](const QString &f)
        {
            return f.contains(tableLink.tableName, Qt::CaseInsensitive);
        });

        if (file != files.end())
        {
            if (Revision.isEmpty())
                args.append(*file);
            else
                args.append(QString("%1/%2").arg(info["url"], *file));
        }

        for (const Link &childlnk : tableLink.links)
        {
            file = std::find_if(files.begin(), files.end(), [=](const QString &f)
            {
                return f.contains(childlnk.tableName, Qt::CaseInsensitive);
            });

            if (file != files.end())
            {
                if (Revision.isEmpty())
                    args.append(*file);
                else
                    args.append(QString("%1/%2").arg(info["url"], *file));
            }
        }
    }

    //if (Action == ActionByLocalDiff)
    {
        if (args.count() > 1)
        {
            QScopedPointer<QProcess> proc(new QProcess);
            proc->setWorkingDirectory(Path);

            CoreStartProcess(proc.data(), "svn.exe", args, true, true);

            DiffData[args.at(1)] = proc->readAllStandardOutput();
        }
    }

    for (const QString &file : filesCleared)
    {
        args = QStringList { "diff" };

        if (!Revision.isEmpty())
        {
            args.append("-c");
            args.append(Revision);
        }

        if (Revision.isEmpty())
            args.append(file);
        else
            args.append(QString("%1/%2").arg(info["url"], file));
        //args.append(info["url"]);

        QScopedPointer<QProcess> proc(new QProcess);
        proc->setWorkingDirectory(Path);

        CoreStartProcess(proc.data(), "svn.exe", args, true, true);

        DiffData[file] = proc->readAllStandardOutput();
    }

    QString connectionString = QString("CONNSTRING=dsn=%1;user id=%2;password=%3")
            .arg(Service, User, Password);

    QStringList argtmpl;
    argtmpl << "--delete"
        << "--insert"
        << "--update"
        << "--cs"
        << connectionString;

    if (IsUnicode)
        argtmpl.append("--unicodedb");

    auto GetResult = [=](const QString &filename) -> QString
    {
        QString result;
        QFile f(filename);

        if (f.open(QIODevice::ReadOnly))
        {
            QTextStream ostream(&f);
            ostream.setCodec("IBM 866");
            ostream.seek(0);

            result = ostream.readAll();
            f.close();
        }

        return result;
    };

    auto ExecDiff = [this, GetResult, argtmpl](const QByteArray &data, const QString &mode) -> QString
    {
        QString resultStr;
        QScopedPointer<QProcess> proc(new QProcess);

        QTemporaryFile tmp;
        QTemporaryFile result;
        if (!tmp.open() || !result.open())
            return QString();

        tmp.write(data);
        tmp.close();
        result.close();

        QStringList arg = argtmpl;
        arg << "--input"
            <<tmp.fileName()
           << "--output"
           << result.fileName()
           << mode;

        int ExitCode = CoreStartProcess(proc.data(), "DiffToScript.exe", arg, true, true);

        if (ExitCode)
        {
            QByteArray errdata = proc->readAllStandardError();
            QTextStream stream(&errdata);
            stream.setCodec("IBM 866");

            QString ModeStr;
            if (mode == "--pg")
                ModeStr = "PostgreSQL";
            else
                ModeStr = "Oracle";

            while (!stream.atEnd())
            {
                QString line = stream.readLine();

                if (line.startsWith("WARNING:"))
                {
                    m_Errors->appendError(QString("%1: %2")
                                          .arg(ModeStr)
                                          .arg(line.remove("WARNING:").simplified()),
                                          ErrorsModel::TypeWarning);
                }
                else if (line.startsWith("Error:"))
                {
                    m_Errors->appendError(QString("%1: %2")
                                          .arg(ModeStr)
                                          .arg(line.remove("WARNING:").simplified()));
                }
                else
                {
                    m_Errors->appendError(QString("%1: %2")
                                          .arg(ModeStr)
                                          .arg(line.simplified()),
                                          ErrorsModel::TypeInfo);
                }
            }
        }

        resultStr = GetResult(result.fileName());
        return resultStr;
    };

    QMapIterator<QString, QByteArray> iter(DiffData);
    while (iter.hasNext())
    {
        iter.next();

        QString result = ExecDiff(iter.value(), "--ora");
        emit oracleScriptReady(result);

        result = ExecDiff(iter.value(), "--pg");
        emit postgresScriptReady(result);
    }

    emit finished();
}

// -------------------------------------------------------------

ScriptsPage::ScriptsPage(QWidget *parent) :
    QWizardPage(parent),
    ui(new Ui::ScriptsPage)
{
    ui->setupUi(this);
    setTitle(tr("Сгенерированные скрипты"));

    m_Errors = new ErrorsModel();
    m_pOracle = new CodeEditor(this);
    m_pPostgres = new CodeEditor(this);

    m_pTabWidget = new QTabWidget(this);
    //m_pSplitter = new QSplitter(Qt::Vertical, this);
    //layout()->addWidget(m_pSplitter);
    layout()->addWidget(m_pTabWidget);

    m_pTabWidget->addTab(m_pOracle, "Oracle");
    m_pTabWidget->addTab(m_pPostgres, "Postgres");
    //m_pSplitter->addWidget(m_pOracle);
    //m_pSplitter->addWidget(m_pPostgres);

    m_pOracle->setReadOnly(true);
    m_pPostgres->setReadOnly(true);

    m_pOracle->setWordWrapMode(QTextOption::WordWrap);
    m_pPostgres->setWordWrapMode(QTextOption::WordWrap);

    ToolApplyHighlighter(m_pOracle, HighlighterSql);
    ToolApplyHighlighter(m_pPostgres, HighlighterSql);
}

ScriptsPage::~ScriptsPage()
{
    delete ui;
}

void ScriptsPage::oracleScriptReady(const QString &data)
{
    m_pOracle->appendPlainText(data);
    m_pOracle->verticalScrollBar()->setValue(m_pOracle->verticalScrollBar()->maximum());
}

void ScriptsPage::postgresScriptReady(const QString &data)
{
    m_pPostgres->appendPlainText(data);
    m_pPostgres->verticalScrollBar()->setValue(m_pPostgres->verticalScrollBar()->maximum());
}

void ScriptsPage::finished()
{
    if (m_Errors->isEmpty())
        return;

    ErrorDlg dlg(ErrorDlg::ModeInformation, this);
    dlg.setErrors(m_Errors);
    dlg.setMessage(tr("При формировании скриптов возникли проблемы:"));
    dlg.exec();
}

void ScriptsPage::initializePage()
{
    DiffWizard *wzrd = qobject_cast<DiffWizard*>(wizard());

    m_pOracle->clear();
    m_pPostgres->clear();
    m_Errors->clear();

    GenerateOperation *oper = new GenerateOperation(wzrd);
    oper->setErrorsBuf(m_Errors);

    connect(oper, &GenerateOperation::oracleScriptReady, this, &ScriptsPage::oracleScriptReady);
    connect(oper, &GenerateOperation::postgresScriptReady, this, &ScriptsPage::postgresScriptReady);
    connect(oper, &GenerateOperation::finished, this, &ScriptsPage::finished);

    QThreadPool::globalInstance()->start(oper);
}
