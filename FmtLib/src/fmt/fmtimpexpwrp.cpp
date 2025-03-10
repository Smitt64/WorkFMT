#include "fmtimpexpwrp.h"
#include "fmtapplication.h"
#include "fmtcore.h"
#include <errorsmodel.h>
#include "loggingcategories.h"
#include <QProcess>
#include <QDir>
#include <QTemporaryDir>
#include <QProgressDialog>
#include <QTextStream>
#include <QRegExpValidator>
#include <QEventLoop>

FmtImpExpWrp::FmtImpExpWrp(ConnectionInfo *connection, QObject *parent) :
    QObject(parent)
{
    m_WorkDir = QApplication::applicationDirPath();
    pConnection = connection;
    pFmtXml = new QProcess(this);

    FmtApplication *app = (FmtApplication*)qApp;
    m_pPrm = app->settings();

    connect(pFmtXml, SIGNAL(finished(int)), SIGNAL(finished(int)));
    connect(pFmtXml, SIGNAL(started()), SIGNAL(started()));
    connect(pFmtXml, SIGNAL(error(QProcess::ProcessError)), SLOT(processError(QProcess::ProcessError)));
    connect(pFmtXml, SIGNAL(readyReadStandardOutput()), SLOT(processReadyReadStandardOutput()));
}

FmtImpExpWrp::~FmtImpExpWrp()
{

}

void FmtImpExpWrp::addTable(const QString &table)
{
    m_Tables.append(table);
}

void FmtImpExpWrp::addTable(const QStringList &tables)
{
    m_Tables.append(tables);
}

void FmtImpExpWrp::clearTables()
{
    m_Tables.clear();
}

void FmtImpExpWrp::removeTable(const QString &table)
{
    m_Tables.removeOne(table);
}

QString FmtImpExpWrp::codePage() const
{
    QString codepage;
    m_pPrm->beginGroup("FmtXml");
    codepage = m_pPrm->value("CodePage", "utf8").toString();
    m_pPrm->endGroup();

    return codepage;
}

bool FmtImpExpWrp::isNoXsd()
{
    bool hr = false;
    m_pPrm->beginGroup("FmtXml");
    hr = m_pPrm->value("NoXsd", "false").toBool();
    m_pPrm->endGroup();
    return hr;
}

QString FmtImpExpWrp::connectionString() const
{
    QString dsn = m_dsn;

    if (dsn.isEmpty())
        dsn = pConnection->dsn();
        //dsn = DatasourceFromService(pConnection->service());

    return QString("dsn=%1;user id=%2;password=%3")
            .arg(dsn)
            .arg(pConnection->user())
            .arg(pConnection->password());
}

bool FmtImpExpWrp::isRunning()
{
    return pFmtXml->state() == QProcess::Running;
}

void FmtImpExpWrp::cancel()
{
    if (isRunning())
        pFmtXml->terminate();
}

void FmtImpExpWrp::processReadyReadStandardOutput()
{
    /*QString s = QString::fromLocal8Bit(pFmtXml->readAll());
    QFile f("1.txt");
    f.open(QIODevice::WriteOnly);
    QTextStream stream(&f);
    stream << s;

    qDebug() << s;
    f.close();*/
}

bool FmtImpExpWrp::isExists(const QString &table)
{
    return m_Tables.contains(table);
}

QString FmtImpExpWrp::programName() const
{
    QString exe = getFullFileNameFromDir("fmtxml.exe");

    if (exe.isEmpty())
    {
        FmtApplication *app = (FmtApplication*)qApp;

        QSettings *m_pPrm = app->settings();
        m_pPrm->beginGroup("FmtXml");
        exe = m_pPrm->value("ExePath", "fmtxml/fmtxml.exe").toString();
        m_pPrm->endGroup();

        QDir d(exe);
        if (!d.isAbsolute())
            exe = getFullFileNameFromDir(exe);
    }

    return exe;
}

void FmtImpExpWrp::processError(QProcess::ProcessError error)
{
    qCInfo(logCore()) << "FmtXml process error: " << GetProcessErrorText(error);
    emit finished(-(10 + error));
}

QString FmtImpExpWrp::protocol() const
{
    QString path = m_Protocol;
    return FileContent(path);
}

void FmtImpExpWrp::setDsn(const QString &dsn)
{
    m_dsn = dsn;
}

void FmtImpExpWrp::parseProtocol(ErrorsModel *ptr)
{
    QFile f866(m_Protocol);
    QFile fUtf(m_Protocol);

    if (f866.open(QIODevice::ReadOnly) && fUtf.open(QIODevice::ReadOnly))
    {
        QRegExp v("\\b[0-9]{2}:[0-9]{2}:[0-9]{2} [0-9]{2}-[0-9]{2}-[0-9]{4}\\b");
        QTextStream stream866(&f866);
        QTextStream streamUtf(&fUtf);
        stream866.setCodec("IBM 866");

        bool IsError = false;
        QString message;
        while(!stream866.atEnd() && !streamUtf.atEnd())
        {
            QString line866 = stream866.readLine();
            QString lineunicode = streamUtf.readLine();

            if (line866.contains(v))
            {
                if (IsError)
                {
                    ptr->appendError(message);
                    message = "";
                    IsError = false;
                }
                QString tmp = line866.mid(20);
                QDateTime dt = QDateTime::fromString(line866, "HH:MM:ss dd-mm-yyyy");

                if (tmp.contains("*** ERROR"))
                {
                    IsError = true;

                    if (tmp.contains("ODBC error"))
                        tmp = lineunicode.mid(20);

                    message += tmp.remove("*** ERROR").trimmed();
                }
                else
                    ptr->appendMessage(tmp, dt);
            }
            else
              message += line866;
        }

        if (!message.isEmpty())
        {
            if (IsError)
                ptr->appendError(message);
            else
                ptr->appendMessage(message);
        }

        f866.close();
        fUtf.close();
    }
}

QString FmtImpExpWrp::FileContent(const QString &filename) const
{
    QString data;
    QFile f(filename);

    if (f.open(QIODevice::ReadOnly))
    {
        data = f.readAll();
        f.close();
    }
    return data;
}

QString FmtImpExpWrp::lastImportDir() const
{
    QString dir;
    m_pPrm->beginGroup("FmtXml");
    dir = m_pPrm->value("LastImportDir", qApp->applicationDirPath()).toString();
    m_pPrm->endGroup();

    return dir;
}

int FmtImpExpWrp::importDir(const QString &impdir)
{
    int stat = 1;
    QStringList arg;
    arg << "/i"
        << QString("-cp:%1").arg(codePage())
        << QString("-cs:%1").arg(connectionString())
        << "/d";

    if (isNoXsd())
        arg << "/n";

    arg << impdir;

    QString exeName = programName();
    if (exeName.isEmpty())
    {
        qCritical(logCore()) << "FmtXml not found";
        return stat;
    }

    m_pPrm->beginGroup("FmtXml");
    m_pPrm->setValue("LastImportDir", impdir);
    m_pPrm->endGroup();

    m_Protocol = impdir + "/protocol.out";
    pFmtXml->setWorkingDirectory(m_WorkDir.path());
    qCInfo(logCore()) << "FmtXml import dir started: " << exeName << arg;
    pFmtXml->start(exeName, arg);
    stat = 0;

    return stat;
}

void FmtImpExpWrp::importFile(const QString &file)
{
    QStringList arg;
    QFileInfo d(m_Tables[0]);
    arg << "/i"
        << QString("-cp:%1").arg(codePage())
        << QString("-cs:%1").arg(connectionString());

    if (isNoXsd())
        arg << "/n";

    arg << file;

    m_pPrm->beginGroup("FmtXml");
    m_pPrm->setValue("LastImportDir", d.absolutePath());
    m_pPrm->endGroup();

    m_Protocol = d.absolutePath() + "/protocol.out";
    pFmtXml->setWorkingDirectory(m_WorkDir.path());
    qCInfo(logCore()) << "FmtXml import file started: " << programName() << arg;
    CoreStartProcess(pFmtXml, programName(), arg, false);
}

void FmtImpExpWrp::getArgs(const QString &dir, QStringList &arg)
{
    arg << QString("-cp:%1").arg(codePage())
        << QString("-cs:%1").arg(connectionString())
        << "/d";

    if (isNoXsd())
        arg << "/n";

    arg << dir
        << m_Tables;
}

void FmtImpExpWrp::exportTable(const QString &dir, bool waitForFinished, bool waitForStarted)
{
    if (dir.isEmpty())
        return;

    QStringList arg;
    getArgs(dir, arg);

    m_Protocol = dir + "/protocol.out";
    pFmtXml->setWorkingDirectory(m_WorkDir.path());
    qCInfo(logCore()) << "FmtXml export table started: " << programName() << arg;
    CoreStartProcess(pFmtXml, programName(), arg, waitForFinished, waitForStarted);
}

int FmtImpExpWrp::tablesCount()
{
    return m_Tables.count();
}
