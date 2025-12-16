#include "operationwizardpage.h"
#include "ui_operationwizardpage.h"
#include <errorsmodel.h>
#include <errorwidget.h>
#include <QVBoxLayout>
#include "regparmwizard.h"

OperationWizardPage::OperationWizardPage(QWidget *parent) :
    QWizardPage(parent),
    ui(new Ui::OperationWizardPage)
{
    ui->setupUi(this);
}

OperationWizardPage::~OperationWizardPage()
{
    delete ui;
}

void OperationWizardPage::initializePage()
{
    RegParmWizard *wzrd = (RegParmWizard*)wizard();

    if (wzrd->selectedAction() == RegParmWizard::ActionAddFromWord)
    {
        setTitle(tr("Создание новых настроек"));
    }
}
