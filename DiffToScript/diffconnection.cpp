#include "diffconnection.h"
#include "fmtcore.h"
#include <QObject>
#include <QApplication>
#include <QSettings>
#include <QFileInfo>
#include <difflogging.h>
#include <regex>

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
    isUnicode = sett.value("connect/isUnicode", "isUnicode").toBool();

    open();
}

DiffConnection::DiffConnection(const QString &connectionString, bool unicode)
{
    const std::regex sqlite_regex("Data Source=(.+);");
    std::smatch match;
    std::string connStr = connectionString.toStdString();

    if (std::regex_match(connStr, match, sqlite_regex))
    {
        std::string m = match[1].str();
        _dsn = QString::fromStdString(m.c_str());
        qCInfo(logDiff) << "Dsn = " << _dsn;
        openSqlite();
        return;
    }

    qCInfo(logDiff) << "Start connect to FMT";

    isUnicode = unicode;
    ParseConnectionString(connectionString, _user, _pass, _dsn);

    open();
}

void DiffConnection::open()
{
    _conn = new ConnectionInfo();
    QString optionStr;

    if (isUnicode)
        optionStr = "RSD_UNICODE";

    qCInfo(logDiff) << "user=" << _user << " pass=" << _pass << " dsn=" << _dsn;
    _connected = _conn->open("qrsd", _user, _pass, _dsn, optionStr);
    if (_connected)
        qCInfo(logDiff) << "Connection " << (_connected?"done":"fail");
    else
        qCWarning(logDiff) << "Connection " << (_connected?"done":"fail");
}

void DiffConnection::openSqlite()
{
    _conn = new ConnectionInfo();
    _connected = _conn->openSqlite(_dsn);
}



