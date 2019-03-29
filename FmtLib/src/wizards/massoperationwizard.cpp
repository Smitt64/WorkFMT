#include "massoperationwizard.h"
#include "src/massop/massopselectoperationpage.h"
#include "src/massop/massopselecttablespage.h"
#include "connectioninfo.h"
#include <QDebug>

MassOperationWizard::MassOperationWizard(ConnectionInfo *info, QWidget *parent) :
    QWizard (parent),
    pInterface(Q_NULLPTR)
{
    pInfo = info;
    setMinimumSize(800, 600);
    addPage(createSelectOperationPage());
    addPage(createSelectTablesPage(info));

    setWizardStyle(QWizard::ClassicStyle);
}

MassOperationWizard::~MassOperationWizard()
{

}

QWizardPage *MassOperationWizard::createSelectOperationPage()
{
    QWizardPage *page = new MassOpSelectOperationPage(this);
    return page;
}

QWizardPage *MassOperationWizard::createSelectTablesPage(ConnectionInfo *info)
{
    QWizardPage *page = new MassOpSelectTablesPage(info, this);
    return page;
}

ConnectionInfo *MassOperationWizard::connectionInfo()
{
    return pInfo;
}

bool MassOperationWizard::addTables(const QString &str)
{
    if (m_Tables.contains(str))
        return false;
    m_Tables.append(str);
    return true;
}

bool MassOperationWizard::removeTables(const QString &str)
{
    m_Tables.removeOne(str);
    return true;
}

QString MassOperationWizard::selectedOpeation() const
{
    MassOpSelectOperationPage *pPage = qobject_cast<MassOpSelectOperationPage*>(page(MassOperationWizard::PageSelectOperation));
    return pPage->selectedItem();
}

const QStringList &MassOperationWizard::tables() const
{
    return m_Tables;
}

void MassOperationWizard::initInterface()
{
    if (pInterface != Q_NULLPTR)
    {
        pInterface->deinitPages();
        delete pInterface;
    }

    MassOpSelectOperationPage *pPage = qobject_cast<MassOpSelectOperationPage*>(page(MassOperationWizard::PageSelectOperation));
    QString interfaceName = pPage->selectedInterface();
    pInterface = MassOpInterface::m_pMassOpInterfaceFactory.create(interfaceName);
    pInterface->setWizard(this);
    pInterface->initPages();
}

MassOpInterface *MassOperationWizard::getInterface()
{
    return pInterface;
}
