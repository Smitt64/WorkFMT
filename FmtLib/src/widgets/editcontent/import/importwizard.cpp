#include "importwizard.h"
#include "selecttypepage.h"
#include "texteditpage.h"
#include "mapfieldspage.h"
#include <QStandardItemModel>

ImportWizard::ImportWizard(FmtSharedTablePtr table, QWidget *parent) :
    QWizard(parent)
{
    pTable = table;
    pTableModel = new QStandardItemModel();
    addPage(createTypePage());
    addPage(createTextEditPage(pTableModel));
    addPage(createMapFieldsPage(pTableModel));
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
    TextEditPage *page = new TextEditPage(pTable, model, this);
    return page;
}

QWizardPage *ImportWizard::createMapFieldsPage(QStandardItemModel *model)
{
    MapFieldsPage *page = new MapFieldsPage(pTable, model, this);
    return page;
}
