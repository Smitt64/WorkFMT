#ifndef IMPORTOBJECT_H
#define IMPORTOBJECT_H

#include <QDir>
#include <QObject>
#include <QScopedPointer>

class ConnectionInfo;
class ImportObject : public QObject
{
    Q_OBJECT
public:
    explicit ImportObject(QObject *parent = nullptr);
    virtual ~ImportObject();

    void setConnectionInfo(const QString &user, const QString &pswd, const QString &dsn, bool unicode);
    void importTable(const QString &datFile, const QDir &workDir);
    void importTables(const QStringList &datFiles, const QDir &workDir);

signals:
    void procMessage(const QString &str);
    void procError(const QString &str);
    void importTableStart(const QString &str);
    void progress(int current, int total);

private:
    QString _user, _pswd, _dsn;
    bool _unicode;
    QScopedPointer<ConnectionInfo> m_conn;
};

#endif // IMPORTOBJECT_H
