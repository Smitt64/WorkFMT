#ifndef DIFFCONNECTION_H
#define DIFFCONNECTION_H

#include "connectioninfo.h"

class DiffConnection
{
public:
    DiffConnection();
    ~DiffConnection() { delete _conn;}
    bool isConnected() const {return _connected;}
    ConnectionInfo* getConnection() const {return _conn;}
    const QString& getUser() const {return _user;}
    const QString& getPass() const {return _pass;}
    const QString& getDsn()  const {return _dsn ;}
private:
    bool _connected;
    ConnectionInfo* _conn;
    QString _user;
    QString _pass;
    QString _dsn ;
};

#endif // DIFFCONNECTION_H
