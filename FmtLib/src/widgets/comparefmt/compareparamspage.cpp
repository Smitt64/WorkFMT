#include "compareparamspage.h"
#include "ui_compareparamspage.h"

CompareParamsPage::CompareParamsPage(QWidget *parent) :
    QWizardPage(parent),
    ui(new Ui::CompareParamsPage)
{
    ui->setupUi(this);
}

CompareParamsPage::~CompareParamsPage()
{
    delete ui;
}
