#include "datpage.h"
#include "ui_datpage.h"

DatPage::DatPage(QWidget *parent) :
    QWizardPage(parent),
    ui(new Ui::DatPage)
{
    ui->setupUi(this);
}

DatPage::~DatPage()
{
    delete ui;
}
