#ifndef DIFFCONNECTION_H
#define DIFFCONNECTION_H

#include <QObject>
#include "connectioninfo.h"

class DiffConnection : public QObject
{
    Q_OBJECT
    Q_PROPERTY(bool isConnected READ isConnected CONSTANT)
    Q_PROPERTY(ConnectionInfo *connectionInfo READ getConnection CONSTANT)
    Q_PROPERTY(QString user READ getUser CONSTANT)
    Q_PROPERTY(QString password READ getPass CONSTANT)
    Q_PROPERTY(QString dsn READ getDsn CONSTANT)
    Q_PROPERTY(bool isUnicode READ IsUnicode CONSTANT)

public:
    DiffConnection();
    DiffConnection(const QString &connectionString, bool unicode);
    ~DiffConnection() { delete _conn;}

    bool isConnected() const {return _connected;}
    ConnectionInfo* getConnection() const {return _conn;}
    const QString& getUser() const {return _user;}
    const QString& getPass() const {return _pass;}
    const QString& getDsn()  const {return _dsn ;}
    const bool& IsUnicode()  const {return isUnicode ;}

private:
    void open();
    void openSqlite();
    bool _connected;
    ConnectionInfo* _conn;
    QString _user;
    QString _pass;
    QString _dsn ;
    bool isUnicode;
};

#endif // DIFFCONNECTION_H
