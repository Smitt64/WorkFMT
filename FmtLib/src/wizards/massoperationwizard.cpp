#include "massoperationwizard.h"
#include "src/massop/massopselectoperationpage.h"
#include "src/massop/massopselecttablespage.h"
#include "connectioninfo.h"

MassOperationWizard::MassOperationWizard(ConnectionInfo *info, QWidget *parent) :
    QWizard (parent)
{
    pInfo = info;
    addPage(createSelectOperationPage());
    addPage(createSelectTablesPage(info));
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
