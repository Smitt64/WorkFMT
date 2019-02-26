#include "mapfieldspage.h"
#include "ui_mapfieldspage.h"
#include "mapfildsmodel.h"

MapFieldsPage::MapFieldsPage(FmtSharedTablePtr table, QStandardItemModel *model, QWidget *parent) :
    QWizardPage(parent),
    ui(new Ui::MapFieldsPage)
{
    ui->setupUi(this);
    pTable = table;
    pModel = model;
    pMapModel = new MapFildsModel(pTable, this);
    ui->tableView->setModel(pMapModel);
}

MapFieldsPage::~MapFieldsPage()
{
    delete ui;
}
