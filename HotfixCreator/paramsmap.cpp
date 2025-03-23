#include "paramsmap.h"

ParamsMap::ParamsMap(const MakeParams &params, QObject *parent) :
    QObject(parent),
    _params(params)
{

}

ParamsMap::~ParamsMap()
{

}

QStringList ParamsMap::keys() const
{
    return _params.keys();
}

bool ParamsMap::contains(const QString &key)
{
    return _params.contains(key);
}

QVariant ParamsMap::value(const QString &key)
{
    return _params.value(key);
}
