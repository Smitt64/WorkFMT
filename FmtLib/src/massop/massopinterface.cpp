#include "massopinterface.h"

FmtLibFactory<MassOpInterface,QString> MassOpInterface::m_pMassOpInterfaceFactory;

MassOpInterface::MassOpInterface(QObject *parent) :
    QObject(parent)
{

}

MassOpInterface::~MassOpInterface()
{

}
