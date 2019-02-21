#include "importwizard.h"
#include "selecttypepage.h"
#include "texteditpage.h"
#include <QStandardItemModel>

ImportWizard::ImportWizard(QWidget *parent) :
    QWizard(parent)
{
    pTableModel = new QStandardItemModel();
    addPage(createTypePage());
    addPage(createTextEditPage(pTableModel));
}

ImportWizard::~ImportWizard()
{

}

QWizardPage *ImportWizard::createTypePage()
{
    SelectTypePage *page = new SelectTypePage(this);
    return page;
}

QWizardPage *ImportWizard::createTextEditPage(QStandardItemModel *model)
{
    TextEditPage *page = new TextEditPage(pTableModel, this);
    return page;
}
