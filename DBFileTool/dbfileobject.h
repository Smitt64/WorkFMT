#ifndef DBFILEOBJECT_H
#define DBFILEOBJECT_H

#include <QObject>
#include <QSettings>
#include <QProcess>
#include <QApplication>
#include <QSettings>
#include <QDir>
#include "fmterrors.h"

Q_GLOBAL_STATIC_WITH_ARGS(QSettings, pSettrings, (QDir(qApp->applicationDirPath()).absoluteFilePath("dbfts.ini"), QSettings::IniFormat))

class ConnectionInfo;
class DBFileObject : public QObject
{
    Q_OBJECT
public:
    explicit DBFileObject(QObject *parent = 0);
    virtual ~DBFileObject();
    QString lastUserName() const;
    QString lastPassword() const;
    QString lastService() const;

    QString exportDir() const;

    void setLastUserName(const QString &str);
    void setLastPassword(const QString &str);
    void setLastService(const QString &str);
    void setExportDir(const QString &str);

signals:
    void procMessage(const QString &str);
    void procInfo(const QString &str);
    void procError(const QString &str);
    void finished();
    void exportTableStart(const QString &str);
    void importTableStart(const QString &str);

public slots:
    void unload(const QString &userName, const QString &password, const QString &service, const QString &ExportDir, const QString &dbt);
    void unload(const QString &userName, const QString &password, const QString &service, const QString &ExportDir, const QStringList &dbt);
    void load(const QString &userName, const QString &password, const QString &service, const QString &dbt);
    void load(const QString &userName, const QString &password, const QString &service, const QStringList &dbt);
    void stop();

private slots:
    void readyReadStandardError();
    void readyReadStandardOutput();
    void error(QProcess::ProcessError error);

private:
    QProcess proc;
    ConnectionInfo *pConnection;
};

#endif // DBFILEOBJECT_H
