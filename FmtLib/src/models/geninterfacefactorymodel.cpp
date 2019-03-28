#include "geninterfacefactorymodel.h"
#include "fmtgeninterface.h"
// FmtGenInterface::m_pGenInterfaceFactory

GenInterfaceFactoryModel::GenInterfaceFactoryModel(QObject *parent) :
    AbstractFactoryModel<FmtGenInterface,QString>(&FmtGenInterface::m_pGenInterfaceFactory, parent)
{

}

GenInterfaceFactoryModel::~GenInterfaceFactoryModel()
{

}
