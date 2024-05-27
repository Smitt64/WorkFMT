#include "scriptspage.h"
#include "ui_scriptspage.h"
#include "codeeditor.h"
#include "fmtcore.h"
#include "diffwizard.h"
#include "task.h"
#include <QProcess>
#include <QThreadPool>
#include <QMapIterator>
#include <QTemporaryFile>
#include <QTextStream>

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

    QMap<QString, QByteArray> DiffData;
    if (Action == ActionByLocalDiff)
    {
        for (const QString &file : files)
        {
            QScopedPointer<QProcess> proc(new QProcess);
            proc->setWorkingDirectory(Path);

            CoreStartProcess(proc.data(), "svn.exe", { "diff", file }, true, true);

            DiffData[file] = proc->readAllStandardOutput();
        }
    }

    //qDebug() << DiffData;

    QString connectionString = QString("CONNSTRING=dsn=%1;user id=%2;password=%3")
            .arg(Service, User, Password);

    QStringList argtmpl;
    QScopedPointer<QProcess> proc(new QProcess);

    argtmpl << "--delete"
        << "--insert"
        << "--update"
        << "--cs"
        << connectionString;

    if (IsUnicode)
        argtmpl.append("--unicodedb");

    auto GetResult = [=](const QString &filename) -> QString
    {

    };

    QMapIterator<QString, QByteArray> iter(DiffData);
    while (iter.hasNext())
    {
        iter.next();

        QTemporaryFile tmp;
        QTemporaryFile result;
        if (!tmp.open() || !result.open())
            continue;

        tmp.write(iter.value());
        tmp.close();
        result.close();

        QStringList arg = argtmpl;
        arg << "--input"
            <<tmp.fileName()
           << "--output"
           << result.fileName();
        CoreStartProcess(proc.data(), "DiffToScript.exe", arg, true, true);

        QFile f(result.fileName());

        if (f.open(QIODevice::ReadOnly))
        {
            QTextStream ostream(&f);
            ostream.setCodec("IBM 866");
            ostream.seek(0);

            emit oracleScriptReady(ostream.readAll());
            f.close();
        }
    }
}

// -------------------------------------------------------------

ScriptsPage::ScriptsPage(QWidget *parent) :
    QWizardPage(parent),
    ui(new Ui::ScriptsPage)
{
    ui->setupUi(this);

    m_pOracle = new CodeEditor(this);
    m_pPostgres = new CodeEditor(this);
    ui->tabWidget->addTab(m_pOracle, tr("Oracle"));
    ui->tabWidget->addTab(m_pPostgres, tr("PostgreSQL"));
}

ScriptsPage::~ScriptsPage()
{
    delete ui;
}

void ScriptsPage::oracleScriptReady(const QString &data)
{
    m_pOracle->appendPlainText(data);
}

void ScriptsPage::initializePage()
{
    DiffWizard *wzrd = qobject_cast<DiffWizard*>(wizard());

    GenerateOperation *oper = new GenerateOperation(wzrd);
    connect(oper, &GenerateOperation::oracleScriptReady, this, &ScriptsPage::oracleScriptReady);

    QThreadPool::globalInstance()->start(oper);
}
