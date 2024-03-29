#include "task.h"
#include "fmtcore.h"
#include "odbctablemodel.h"
#include "tnsnames.h"
#include "toolsruntime.h"
#include <QSqlQuery>
#include <QFile>
#include <QProcess>
#include <QTextStream>
#include <QTemporaryDir>

Task::Task(int argc, char *argv[], QObject *parent)
    : QObject(parent),
      m_Offset(0)
{
    static QStringList Actions =
    {
        "impdp",
        "expdp",
        "pg_dump"
    };

    for (int i = 1; i < argc; i++)
    {
        QString arg = argv[i];

        if (!Actions.contains(arg))
            m_Params.append(arg);
        else
            m_Action = arg;

        if (arg.contains('@') && arg.contains('/'))
        {
            int pos1 = arg.indexOf('/');
            int pos2 = arg.indexOf('@');

            m_SystemName = arg.mid(0, pos1);
            m_SystemPass = arg.mid(pos1 + 1, pos2 - pos1 - 1);
            m_Service = arg.mid(pos2 + 1);
        }

        if (arg.contains("directory"))
            m_Directory = arg.split('=').at(1);

        if (arg.contains("logfile"))
            m_LogFile = arg.split('=').at(1);
    }
}

bool Task::isOracle()
{
    return m_Action == "impdp" || m_Action == "expdp";
}

bool Task::isPostgres()
{
    return m_Action == "pg_dump";
}

bool Task::SetupOra()
{
    QSqlDatabase db = QSqlDatabase::addDatabase(QRSD_DRIVER);
    db.setUserName(m_SystemName);
    db.setPassword(m_SystemPass);
    db.setDatabaseName(m_Service);

    if (db.open())
    {
        QSqlQuery query("select directory_name, directory_path from dba_directories where directory_name = ?", db);
        query.addBindValue(m_Directory);

        if (!ExecuteQuery(&query))
        {
            OdbcTableModel DsnModel;
            QModelIndex index = DsnModel.indexOfService(m_Service);
            m_OraHome = DsnModel.data(DsnModel.index(index.row(), OdbcTableModel::fld_OraHome)).toString();

            TnsNames names(m_OraHome);
            QString host = names.element(m_Service)._Host;

            if (query.next())
            {
                QString directory_path = query.value("directory_path").toString().mid(2);
                m_DumpDir = QString("\\\\%1%2").arg(host, directory_path);
            }
        }
    }
    else
    {
        emit finished();
        return false;
    }

    QString FullLogFile = m_DumpDir.absoluteFilePath(m_LogFile);
    if (QFile::exists(FullLogFile))
        QFile::remove(FullLogFile);

    m_Process.reset(new QProcess());
    m_Process->setWorkingDirectory(m_OraHome);

    m_Action = QString("%1.exe").arg(m_Action);

    return true;
}

bool Task::SetupPostgres()
{
    QString ActionParam = m_Action;
    m_TempDir.reset(new QTemporaryDir());
    m_DumpDir = m_TempDir->path();
    m_LogFile = "pg_op.txt";
    m_Action = "pg_op.cmd";
    QFile f(m_DumpDir.absoluteFilePath(m_Action));

    if (!f.open(QIODevice::WriteOnly))
        return false;

    QDir PostgresDir;
    if (!toolGetPostgreSQLInstallLocation(PostgresDir))
        return false;

    QTextStream stream(&f);
    //stream << "chcp 1251" << Qt::endl;
    stream << "call \"";
    stream << PostgresDir.absoluteFilePath(ActionParam) << ".exe\"";

    bool NeedAddBracers = false;
    for (const QString &prm : m_Params)
    {
        if (!NeedAddBracers)
            stream << " " << prm;
        else
        {
            stream << " \"" << prm << "\"";
            NeedAddBracers = false;
        }

        if (prm == "-f" || prm == "-h")
            NeedAddBracers = true;

    }

    stream << " 2>\"" << m_DumpDir.absoluteFilePath("pg_op.txt") << "\"";
    f.close();
    m_Params.clear();

    m_Process.reset(new QProcess());
    m_Process->setWorkingDirectory(m_DumpDir.path());

    m_Action = "cmd.exe";
    m_Params.append({"/C", f.fileName()});

    return true;
}

void Task::run()
{
    if (isOracle() && !SetupOra())
        return;

    if (isPostgres() && !SetupPostgres())
        return;

    m_Timer.reset(new QTimer());
    m_Timer->setInterval(1000);
    connect(m_Timer.data(), &QTimer::timeout, this, &Task::onTimeOut);

    connect(m_Process.data(), SIGNAL(started()), m_Timer.data(), SLOT(start()));
    connect(m_Process.data(), qOverload<int, QProcess::ExitStatus>(&QProcess::finished), [=](int exitCode, QProcess::ExitStatus exitStatus)
    {
        m_Timer->stop();
        emit finished();
    });

    m_Timer->start();
    CoreStartProcess(m_Process.data(), m_Action,
                     m_Params, false, true,
                     std::numeric_limits<int>::max());
}

void Task::onTimeOut()
{
    QTextStream out(stdout);
    out.setCodec("IBM 866");
    QFile f(m_DumpDir.absoluteFilePath(m_LogFile));

    if (f.open(QIODevice::ReadOnly))
    {
        qint64 size = f.size();

        if (size > m_Offset)
        {
            f.seek(m_Offset);
            QTextStream stream(&f);
            stream.setCodec("Windows-1251");
            while (!stream.atEnd())
            {
                QString line = stream.readLine();
                out << QString("%1\n").arg(line);
                out.flush();
            }
            m_Offset = size;
        }
        f.close();
    }
}
