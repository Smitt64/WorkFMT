#include "massinittableparams.h"
#include "ui_massinittableparams.h"
#include "massinittablesparammodel.h"
#include "massoperationwizard.h"
#include "../massinittableoperation.h"

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
    MassOperationWizard *wzrd = qobject_cast<MassOperationWizard*>(wizard());

    if (!wzrd)
        return;

    MassInitTableOperation *pInterface = qobject_cast<MassInitTableOperation*>(wzrd->getInterface());

    if (pModel == Q_NULLPTR)
    {
        pModel = pInterface->model();
        pModel->setTables(wzrd->tables());
        ui->tableView->setModel(pModel);
        ui->tableView->setColumnWidth(MassInitTablesParamModel::FieldInitTable, 200);
        ui->tableView->setColumnWidth(MassInitTablesParamModel::FieldInitIndex, 200);
        ui->tableView->horizontalHeader()->setSectionResizeMode(MassInitTablesParamModel::FieldTableName, QHeaderView::Stretch);
    }
    else
    {
        pModel->setTables(wzrd->tables());
    }
}

void MassInitTableParams::initializePage()
{
    MassOperationWizard *wzrd = qobject_cast<MassOperationWizard*>(wizard());
    setTables(wzrd->tables());
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
