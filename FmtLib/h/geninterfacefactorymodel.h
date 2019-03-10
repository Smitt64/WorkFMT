#ifndef GENINTERFACEFACTORY_H
#define GENINTERFACEFACTORY_H

#include <QObject>
#include "fmtgeninterface.h"
#include "src/core/abstractfactorymodel.hpp"

class GenInterfaceFactoryModel : public AbstractFactoryModel<FmtGenInterface,QString>
{
    Q_OBJECT
public:
    GenInterfaceFactoryModel(QObject *parent = Q_NULLPTR);
    virtual ~GenInterfaceFactoryModel();
};

#endif // GENINTERFACEFACTORY_H
