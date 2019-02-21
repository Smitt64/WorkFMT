#include "dbfileobject.h"
#include <fmtcore.h>
#include <connectioninfo.h>
#include <QDebug>
#include <QTemporaryDir>

DBFileObject::DBFileObject(QObject *parent) :
    QObject(parent)
{
    connect(&proc, SIGNAL(readyReadStandardError()), SLOT(readyReadStandardError()));
    connect(&proc, SIGNAL(readyReadStandardOutput()), SLOT(readyReadStandardOutput()));
    connect(&proc, SIGNAL(error(QProcess::ProcessError)), SLOT(error(QProcess::ProcessError)));
}

DBFileObject::~DBFileObject()
{

}

QString DBFileObject::lastUserName() const
{
    QString value;
    pSettrings->beginGroup("LastConnection");
    value = pSettrings->value("UserName").toString();
    pSettrings->endGroup();
    return value;
}

QString DBFileObject::lastPassword() const
{
    QString value;
    pSettrings->beginGroup("LastConnection");
    value = pSettrings->value("Password").toString();
    pSettrings->endGroup();
    return value;
}

QString DBFileObject::lastService() const
{
    QString value;
    pSettrings->beginGroup("LastConnection");
    value = pSettrings->value("Service").toString();
    pSettrings->endGroup();
    return value;
}

void DBFileObject::setLastUserName(const QString &str)
{
    pSettrings->beginGroup("LastConnection");
    pSettrings->setValue("UserName", str);
    pSettrings->endGroup();
    pSettrings->sync();
}

void DBFileObject::setLastPassword(const QString &str)
{
    pSettrings->beginGroup("LastConnection");
    pSettrings->setValue("Password", str);
    pSettrings->endGroup();
    pSettrings->sync();
}

void DBFileObject::setLastService(const QString &str)
{
    pSettrings->beginGroup("LastConnection");
    pSettrings->setValue("Service", str);
    pSettrings->endGroup();
    pSettrings->sync();
}

QString DBFileObject::exportDir() const
{
    QString value;
    value = pSettrings->value("ExportDir").toString();
    return value;
}

void DBFileObject::setExportDir(const QString &str)
{
    pSettrings->setValue("ExportDir", str);
    pSettrings->sync();
}

void DBFileObject::readyReadStandardError()
{
    QString err = proc.readAllStandardError();
    emit procMessage(err);
    emit procError(err);
}

void DBFileObject::readyReadStandardOutput()
{
    QString msg = proc.readAllStandardOutput();
    if (msg.contains(QRegExp("ORA-\\d{5}[^\\n]*")))
        emit procError(msg.simplified());
    emit procMessage(msg);
}

void DBFileObject::error(QProcess::ProcessError error)
{
    emit procError(GetProcessErrorText(error));
}

void DBFileObject::stop()
{
    if (proc.state() == QProcess::Running)
        proc.kill();
}

void DBFileObject::unload(const QString &userName, const QString &password, const QString &service, const QString &ExportDir, const QString &dbt)
{
    unload(userName, password, service, ExportDir, QStringList() << dbt);
}

void DBFileObject::unload(const QString &userName, const QString &password, const QString &service, const QString &ExportDir, const QStringList &dbt)
{
    QEventLoop loop;
    connect(&proc, SIGNAL(finished(int)), &loop, SLOT(quit()));
    QTemporaryDir tmpdir;
    QString programmPath;
    QDir tmpDir(tmpdir.path());

    QProcessEnvironment env = QProcessEnvironment::systemEnvironment();
    proc.setEnvironment(env.toStringList());
    proc.setWorkingDirectory(ExportDir);
    programmPath = tmpDir.absoluteFilePath("RSexp.exe");
    qDebug() << "Temp directory:" << tmpdir.path();
    qDebug() << "Copy RSexp:" << QFile::copy(":/RSexp" , programmPath);
    qDebug() << "RSexp path:" << programmPath;

    foreach (const QString &table, dbt) {
        proc.setArguments(QStringList() << userName << password << service << table);
        proc.setProgram(tmpDir.absoluteFilePath("RSexp.exe"));
        proc.start();

        if (proc.waitForStarted())
        {
            emit importTableStart(table);
            emit procMessage("*************** Start unloading ***************");
            loop.exec();
        }
    }
    qDebug() << "Temp directory remove:" << tmpdir.remove();
}

void DBFileObject::load(const QString &userName, const QString &password, const QString &service, const QString &dbt)
{
    load(userName, password, service, QStringList() << dbt);
}

void DBFileObject::load(const QString &userName, const QString &password, const QString &service, const QStringList &dbt)
{
    QEventLoop loop;
    connect(&proc, SIGNAL(finished(int)), &loop, SLOT(quit()));

    QProcessEnvironment env = QProcessEnvironment::systemEnvironment();
    env.insert("NLS_NUMERIC_CHARACTERS", ".,");
    env.insert("NLS_LANG", "AMERICAN_CIS.RU8PC866");
    proc.setEnvironment(env.toStringList());

    QTemporaryDir tmpdir;
    QDir tmpDir(tmpdir.path());
    QString programmPath = tmpDir.absoluteFilePath("FillTable.sql");
    qDebug() << "Temp directory:" << tmpdir.path();
    qDebug() << "Copy \"FillTable.sql\":" << QFile::copy(":/FillTable" , programmPath);
    qDebug() << "\"FillTable.sql\" path:" << programmPath;

    foreach (const QString &ImportFile, dbt) {
        QFileInfo info(ImportFile);
        QString table = info.baseName();
        QString workDirectory = info.absoluteDir().path();

        qDebug() << "Working directory:" << workDirectory;
        qDebug() << "Programm Path:" << programmPath;
        qDebug() << "Table:" << table;
        proc.setWorkingDirectory(workDirectory);
        proc.setArguments(QStringList()
                          << QString("%1/%2@%3").arg(userName, password, service)
                          << QString("@%1").arg(programmPath)
                          << table
                          << table
                          << userName
                          << password
                          << service);
        proc.setProgram("sqlplus");
        proc.start();

        if (proc.waitForStarted())
        {
            emit exportTableStart(table);
            emit procMessage("**************** Start loading ****************");
            loop.exec();

            QString logFile = QDir(workDirectory).absoluteFilePath(table + ".log");
            QString log = ReadTextFileContent(logFile);
            emit procInfo(log);
        }
    }
    qDebug() << "Temp directory remove:" << tmpdir.remove();
}
