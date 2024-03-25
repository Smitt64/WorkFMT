#include "diffconnection.h"

#include <QObject>
#include <QApplication>
#include <QSettings>
#include <QFileInfo>
#include <difflogging.h>

DiffConnection::DiffConnection()
{
    qCInfo(logDiff) << "Start connect to FMT";

    QFileInfo current(QCoreApplication::applicationFilePath());
    QSettings sett(current.path() + "/DiffToScript.ini", QSettings::IniFormat);

    if (!sett.contains("connect/user"))
        sett.setValue("connect/user", "user");
    if (!sett.contains("connect/pass"))
        sett.setValue("connect/pass", "pass");
    if (!sett.contains("connect/dsn"))
        sett.setValue("connect/dsn", "dsn");
    sett.sync();

    _user = sett.value("connect/user", "user").toString();
    _pass = sett.value("connect/pass", "pass").toString();
    _dsn = sett.value("connect/dsn", "dsn").toString();

    _conn = new ConnectionInfo();
    QString optionStr;
    qCInfo(logDiff) << "user=" << _user << " pass=" << _pass << " dsn=" << _dsn;
    _connected = _conn->open("qrsd", _user, _pass, _dsn, optionStr);
    if (_connected)
        qCInfo(logDiff) << "Connection " << (_connected?"done":"fail");
    else
        qCWarning(logDiff) << "Connection " << (_connected?"done":"fail");
}
