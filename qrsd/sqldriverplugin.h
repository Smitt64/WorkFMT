#ifndef SQLDRIVERPLUGIN_H
#define SQLDRIVERPLUGIN_H

#include <QSqlDriverPlugin>

class SqlDriverPlugin : public QSqlDriverPlugin
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID QSqlDriverFactoryInterface_iid FILE "qrsd.json")

public:
    explicit SqlDriverPlugin(QObject *parent = nullptr);

private:
    QSqlDriver *create(const QString &key) override;
};

#endif // SQLDRIVERPLUGIN_H
