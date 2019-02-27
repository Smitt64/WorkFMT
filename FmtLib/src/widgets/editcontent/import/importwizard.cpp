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

    QWizardPage *pTypePage = createTypePage();
    QWizardPage *pTextEditPage = createTextEditPage(pTableModel);
    QWizardPage *pMapFieldsPage = createMapFieldsPage(pTableModel);
    static_cast<MapFieldsPage*>(pMapFieldsPage)->setTextEditPage(qobject_cast<TextEditPage*>(pTextEditPage));

    addPage(pTypePage);
    addPage(pTextEditPage);
    addPage(pMapFieldsPage);
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
