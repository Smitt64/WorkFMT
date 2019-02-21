#include "selecttypepage.h"
#include "ui_selecttypepage.h"
#include "importwizard.h"

SelectTypePage::SelectTypePage(QWidget *parent) :
    QWizardPage(parent),
    ui(new Ui::SelectTypePage)
{
    ui->setupUi(this);
}

SelectTypePage::~SelectTypePage()
{
    delete ui;
}

int SelectTypePage::nextId() const
{
    if (ui->radioButton->isChecked())
        return page_TextEdit;
    return 0;
}
