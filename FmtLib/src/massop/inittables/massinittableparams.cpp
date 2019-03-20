#include "massinittableparams.h"
#include "ui_massinittableparams.h"
#include "massinittablesparammodel.h"
#include "massoperationwizard.h"

MassInitTableParams::MassInitTableParams(QWidget *parent) :
    QWizardPage(parent),
    ui(new Ui::MassInitTableParams),
    pModel(Q_NULLPTR)
{
    ui->setupUi(this);
    connect(ui->pushButton, SIGNAL(clicked()), SLOT(selectAllCreateTables()));
    connect(ui->pushButton_2, SIGNAL(clicked()), SLOT(selectAllCreateIndex()));
}

MassInitTableParams::~MassInitTableParams()
{
    delete ui;
}

void MassInitTableParams::setTables(const QStringList &list)
{
    if (pModel == Q_NULLPTR)
    {
        pModel = new MassInitTablesParamModel(list, this);
        ui->tableView->setModel(pModel);
        ui->tableView->setColumnWidth(MassInitTablesParamModel::FieldInitTable, 200);
        ui->tableView->setColumnWidth(MassInitTablesParamModel::FieldInitIndex, 200);
        ui->tableView->horizontalHeader()->setSectionResizeMode(MassInitTablesParamModel::FieldTableName, QHeaderView::Stretch);
    }
    else
    {
        MassOperationWizard *wzrd = qobject_cast<MassOperationWizard*>(wizard());
        pModel->setTables(wzrd->tables());
    }
}

void MassInitTableParams::initializePage()
{
    MassOperationWizard *wzrd = qobject_cast<MassOperationWizard*>(wizard());
    setTables(wzrd->tables());
    //ui->tableView->horizontalHeader().setSe
}

void MassInitTableParams::selectAllCreateTables()
{
    if (pModel)
    {
        for (int i = 0; i < pModel->rowCount(QModelIndex()); i++)
        {
            pModel->setData(pModel->index(i, MassInitTablesParamModel::FieldInitTable), Qt::Checked, Qt::CheckStateRole);
        }
        ui->tableView->update();
    }
}

void MassInitTableParams::selectAllCreateIndex()
{
    if (pModel)
    {
        for (int i = 0; i < pModel->rowCount(QModelIndex()); i++)
        {
            pModel->setData(pModel->index(i, MassInitTablesParamModel::FieldInitIndex), Qt::Checked, Qt::CheckStateRole);
        }
        ui->tableView->update();
    }
}
