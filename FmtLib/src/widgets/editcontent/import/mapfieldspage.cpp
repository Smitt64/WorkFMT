#include "mapfieldspage.h"
#include "ui_mapfieldspage.h"
#include "mapfildsmodel.h"
#include "texteditpage.h"
#include <QDebug>

MapFieldsPage::MapFieldsPage(FmtSharedTablePtr table, QStandardItemModel *model, QWidget *parent) :
    QWizardPage(parent),
    ui(new Ui::MapFieldsPage),
    pTextPage(Q_NULLPTR)
{
    ui->setupUi(this);
    pTable = table;
    pModel = model;
    pMapModel = new MapFildsModel(pTable, this);
    ui->tableView->setModel(pMapModel);
    ui->tableView_2->setModel(pModel);
}

MapFieldsPage::~MapFieldsPage()
{
    delete ui;
}

void MapFieldsPage::setTextEditPage(TextEditPage *page)
{
    pTextPage = page;
    connect(pTextPage, SIGNAL(tableChanged()), this, SLOT(tableChanged()));
}

void MapFieldsPage::tableChanged()
{
    for (int i = 0; i < pTable->fieldsCount(); i++)
    {
        int col = pTextPage->getColumnForFmtField(i);
        if (col != -1)
            pMapModel->setData(pMapModel->index(0, 1), QString("${%1}").arg(col));
    }
}
