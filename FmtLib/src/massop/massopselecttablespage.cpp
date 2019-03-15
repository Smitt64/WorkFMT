#include "massopselecttablespage.h"
#include "tablesselectwidget.h"
#include "connectioninfo.h"
#include "massoperationwizard.h"
#include <functional>

MassOpSelectTablesPage::MassOpSelectTablesPage(ConnectionInfo *info, QWidget *parent) :
    QWizardPage(parent)
{
    pLayout = new QVBoxLayout;
    //MassOperationWizard *pWizard = qobject_cast<MassOperationWizard*>(wizard());
    pSeletTables = new TablesSelectWidget(info, this);

    pLayout->addWidget(pSeletTables);
    setLayout(pLayout);
    setTitle(tr("Выбор таблиц"));

    pSeletTables->setAddFunc(std::bind(&MassOpSelectTablesPage::TablesSelectAddFunc, this, std::placeholders::_1));
}

MassOpSelectTablesPage::~MassOpSelectTablesPage()
{

}

bool MassOpSelectTablesPage::TablesSelectAddFunc(const QString &str)
{
    MassOperationWizard *pWizard = qobject_cast<MassOperationWizard*>(wizard());
    return pWizard->addTables(str);
}
