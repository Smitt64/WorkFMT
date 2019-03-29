#include "massopinterface.h"
#include "massoperationwizard.h"

FmtLibFactory<MassOpInterface,QString> MassOpInterface::m_pMassOpInterfaceFactory;

MassOpInterface::MassOpInterface(QObject *parent) :
    QObject(parent),
    pWizard(Q_NULLPTR),
    pInfo(NULL)
{

}

MassOpInterface::~MassOpInterface()
{

}

void MassOpInterface::setWizard(MassOperationWizard *wzrd)
{
    pWizard = wzrd;
}

int MassOpInterface::addPage(QWizardPage *page)
{
    int id = pWizard->addPage(page);
    m_PageIds.append(id);
    return id;
}

void MassOpInterface::deinitPages()
{
    foreach (const int &id, m_PageIds) {
        pWizard->removePage(id);
    }
}

void MassOpInterface::setConnectionInfo(ConnectionInfo *info)
{
    pInfo = info;
}

ConnectionInfo *MassOpInterface::connectionInfo()
{
    return pInfo;
}
