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
