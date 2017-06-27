#include "dbfileobject.h"
#include <fmtcore.h>
#include <connectioninfo.h>
#include <QDebug>
#include <QTemporaryDir>

Q_GLOBAL_STATIC_WITH_ARGS(QSettings, pSettrings, (QDir(qApp->applicationDirPath()).absoluteFilePath("dbfts.ini"), QSettings::IniFormat))

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
    emit procMessage(proc.readAllStandardOutput());
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
    QDir tmpDir(tmpdir.path());

    proc.setWorkingDirectory(ExportDir);
    qDebug() << "Temp directory:" << tmpdir.path();
    qDebug() << "Copy RSexp:" << QFile::copy(":/RSexp" , tmpDir.absoluteFilePath("RSexp.exe"));
    qDebug() << "RSexp path:" << tmpDir.absoluteFilePath("RSexp.exe");

    foreach (const QString &table, dbt) {
        proc.setArguments(QStringList() << userName << password << service << table);
        proc.setProgram(tmpDir.absoluteFilePath("RSexp.exe"));
        proc.start();

        if (proc.waitForStarted())
        {
            emit exportTableStart(table);
            emit procMessage("*************** Start unloading ***************");
            loop.exec();
        }
    }
}
