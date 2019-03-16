#include "massopselecttablespage.h"
#include "tablesselectwidget.h"
#include "connectioninfo.h"
#include "massoperationwizard.h"
#include <functional>

MassOpSelectTablesPage::MassOpSelectTablesPage(ConnectionInfo *info, QWidget *parent) :
    QWizardPage(parent)
{
    pLayout = new QVBoxLayout;
    pSeletTables = new TablesSelectWidget(info, this);

    pLayout->addWidget(pSeletTables);
    setLayout(pLayout);

    pSeletTables->setAddFunc(std::bind(&MassOpSelectTablesPage::AddFunc, this, std::placeholders::_1));
    pSeletTables->setRemFunc(std::bind(&MassOpSelectTablesPage::RemoveFunc, this, std::placeholders::_1));
}

MassOpSelectTablesPage::~MassOpSelectTablesPage()
{

}

bool MassOpSelectTablesPage::AddFunc(const QString &str)
{
    MassOperationWizard *pWizard = qobject_cast<MassOperationWizard*>(wizard());
    return pWizard->addTables(str);
}

bool MassOpSelectTablesPage::RemoveFunc(const QString &str)
{
    MassOperationWizard *pWizard = qobject_cast<MassOperationWizard*>(wizard());
    return pWizard->removeTables(str);
}

void MassOpSelectTablesPage::initializePage()
{
    MassOperationWizard *pWizard = qobject_cast<MassOperationWizard*>(wizard());
    setTitle(tr("Выбор таблиц: %1").arg(pWizard->selectedOpeation()));
    pWizard->initInterface();
    //QString interfaceName = pWizard->selectedInterface();
}
