#include "sqldriverplugin.h"
#include "rsddriver.h"

SqlDriverPlugin::SqlDriverPlugin(QObject *parent)
    : QSqlDriverPlugin(parent)
{
    qRegisterMetaType<RsdDriver*>();
}

QSqlDriver *SqlDriverPlugin::create(const QString &key)
{
    //static_assert(false, "You need to implement this function");
    if (key == "qrsd")
        return new RsdDriver();

    return nullptr;
}
