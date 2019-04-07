#include "massdestribparamspage.h"
#include "ui_massdestribparamspage.h"
#include "massoperationwizard.h"
#include "massdestribcreate.h"
#include "massdestribparammodel.h"

MassDestribParamsPage::MassDestribParamsPage(QWidget *parent) :
    QWizardPage(parent),
    ui(new Ui::MassDestribParamsPage)
{
    ui->setupUi(this);
}

MassDestribParamsPage::~MassDestribParamsPage()
{
    delete ui;
}

void MassDestribParamsPage::initializePage()
{
    MassOperationWizard *wzrd = qobject_cast<MassOperationWizard*>(wizard());
    MassDestribCreate *inter = qobject_cast<MassDestribCreate*>(wzrd->getInterface());

    ui->treeView->setModel(inter->model());
    inter->model()->setTables(wzrd->tables());
    ui->treeView->expandAll();
}
