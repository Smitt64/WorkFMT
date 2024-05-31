#include "scriptspage.h"
#include "ui_scriptspage.h"
#include "codeeditor.h"
#include "fmtcore.h"
#include "toolsruntime.h"
#include "diffwizard.h"
#include "tablelinks.h"
#include "task.h"
#include <QProcess>
#include <QThreadPool>
#include <QMapIterator>
#include <QTemporaryFile>
#include <QTextStream>
#include <QSplitter>
#include <QScrollBar>

enum
{
    ActionByLocalDiff = 0
};

GenerateOperation::GenerateOperation(DiffWizard *wizrd) :
    QObject(),
    QRunnable()
{
    m_pWzrd = wizrd;
    setAutoDelete(true);
}

GenerateOperation::~GenerateOperation()
{

}
// --delete --insert --update --cs "CONNSTRING=dsn=THOR_DB12DEV1;user id=SERP_3188;password=SERP_3188" --input diff.txt
void GenerateOperation::run()
{
    int Action = m_pWzrd->field("Action").toInt();
    QString Path = m_pWzrd->field("Path").toString();
    QString User = m_pWzrd->field("User").toString();
    QString Password = m_pWzrd->field("Password").toString();
    QString Service = m_pWzrd->field("Service").toString();
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
    for (const TableLinks &tableLink : tableLinks)
    {
        QStringList::iterator file = std::find_if(files.begin(), files.end(), [=](const QString &f)
        {
            return f.contains(tableLink.tableName, Qt::CaseInsensitive);
        });

        if (file != files.end())
            args.append(*file);

        for (const Link &childlnk : tableLink.links)
        {
            file = std::find_if(files.begin(), files.end(), [=](const QString &f)
            {
                return f.contains(childlnk.tableName, Qt::CaseInsensitive);
            });

            if (file != files.end())
                args.append(*file);
        }
    }

    if (Action == ActionByLocalDiff)
    {
        if (args.count() > 1)
        {
            QScopedPointer<QProcess> proc(new QProcess);
            proc->setWorkingDirectory(Path);

            args.append(files);
            CoreStartProcess(proc.data(), "svn.exe", args, true, true);

            DiffData[args.at(1)] = proc->readAllStandardOutput();
        }
    }

    for (const QString &file : filesCleared)
    {
        args = QStringList { "diff" };
        args.append(file);

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

    auto ExecDiff = [=](const QByteArray &data, const QString &mode) -> QString
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
        CoreStartProcess(proc.data(), "DiffToScript.exe", arg, true, true);

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
}

// -------------------------------------------------------------

ScriptsPage::ScriptsPage(QWidget *parent) :
    QWizardPage(parent),
    ui(new Ui::ScriptsPage)
{
    ui->setupUi(this);
    setTitle(tr("Сгенерированные скрипты"));

    m_pOracle = new CodeEditor(this);
    m_pPostgres = new CodeEditor(this);

    m_pSplitter = new QSplitter(Qt::Vertical, this);
    layout()->addWidget(m_pSplitter);

    m_pSplitter->addWidget(m_pOracle);
    m_pSplitter->addWidget(m_pPostgres);

    m_pOracle->setReadOnly(true);
    m_pPostgres->setReadOnly(true);

    m_pOracle->setWordWrapMode(QTextOption::WordWrap);
    m_pPostgres->setWordWrapMode(QTextOption::WordWrap);

    m_pOracleHighlighter = new Highlighter(Highlighter::HC_SQL, m_pOracle->document());
    m_pPostgresHighlighter = new Highlighter(Highlighter::HC_SQL, m_pPostgres->document());
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

void ScriptsPage::initializePage()
{
    DiffWizard *wzrd = qobject_cast<DiffWizard*>(wizard());

    m_pOracle->clear();
    m_pPostgres->clear();

    GenerateOperation *oper = new GenerateOperation(wzrd);
    connect(oper, &GenerateOperation::oracleScriptReady, this, &ScriptsPage::oracleScriptReady);
    connect(oper, &GenerateOperation::postgresScriptReady, this, &ScriptsPage::postgresScriptReady);

    QThreadPool::globalInstance()->start(oper);
}
