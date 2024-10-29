#include "sqldriverplugin.h"
#include "rsddriver.h"
#include "rsdcore.h"
#include <QDebug>

Q_GLOBAL_STATIC(CRsdCoreLoader, CoreLoader)

SqlDriverPlugin::SqlDriverPlugin(QObject *parent)
    : QSqlDriverPlugin(parent)
{
    qRegisterMetaType<RsdDriver*>();
}

QSqlDriver *SqlDriverPlugin::create(const QString &key)
{
    if (!CoreLoader->isLoaded())
    {
        if (!CoreLoader->load())
            return nullptr;
    }

    if (key.toUpper() == "QRSD")
        return new RsdDriver();

    return nullptr;
}
