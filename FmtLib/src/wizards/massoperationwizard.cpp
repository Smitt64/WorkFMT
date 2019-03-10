#include "massoperationwizard.h"
#include "src/massop/massopselectoperationpage.h"

MassOperationWizard::MassOperationWizard(QWidget *parent) :
    QWizard (parent)
{
    addPage(createSelectOperationPage());
}

MassOperationWizard::~MassOperationWizard()
{

}

QWizardPage *MassOperationWizard::createSelectOperationPage()
{
    QWizardPage *page = new MassOpSelectOperationPage(this);
    return page;
}
