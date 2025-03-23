#ifndef PARAMSMAP_H
#define PARAMSMAP_H

#include <QObject>
#include "model/contenttreeitem.h"
#include <functional>

class ParamsMap : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QStringList keys READ keys CONSTANT)
public:
    ParamsMap(const MakeParams &params, QObject *parent = nullptr);
    virtual ~ParamsMap();

    QStringList keys() const;
    Q_INVOKABLE bool contains(const QString &key);
    Q_INVOKABLE QVariant value(const QString &key);

private:
    MakeParams _params;
};

#endif // PARAMSMAP_H
