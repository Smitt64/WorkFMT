#include "massdestribparamspage.h"
#include "ui_massdestribparamspage.h"
#include "massoperationwizard.h"
#include "massdestribcreate.h"
#include "massdestribparammodel.h"
#include "massdestribitemparamdelegate.h"
#include <QFileDialog>
#include <QAction>

MassDestribParamsPage::MassDestribParamsPage(QWidget *parent) :
    QWizardPage(parent),
    ui(new Ui::MassDestribParamsPage)
{
    ui->setupUi(this);
    pDelegate = new MassDestribItemParamDelegate(this);
    ui->treeView->setItemDelegate(pDelegate);
    connect(ui->toolButton, &QToolButton::clicked, this, &MassDestribParamsPage::selectFolder);
}

MassDestribParamsPage::~MassDestribParamsPage()
{
    delete ui;
}

void MassDestribParamsPage::initializePage()
{
    MassOperationWizard *wzrd = qobject_cast<MassOperationWizard*>(wizard());
    MassDestribCreate *inter = qobject_cast<MassDestribCreate*>(wzrd->getInterface());

    ui->treeView->setModel(inter->model());
    inter->model()->setTables(wzrd->tables());
    ui->treeView->expandAll();

    ui->treeView->header()->resizeSection(MassDestribParamModel::fld_Name, 200);
    ui->treeView->header()->resizeSection(MassDestribParamModel::fld_Action, 150);
    ui->treeView->header()->resizeSection(MassDestribParamModel::fld_UnloadFmt, 125);
    ui->treeView->header()->resizeSection(MassDestribParamModel::fld_UnloadCreateTables, 135);

    connect(inter->model(), &MassDestribParamModel::rowsInserted, ui->treeView, &QTreeView::expandAll);
    connect(inter->model(), &MassDestribParamModel::rowsMoved, ui->treeView, &QTreeView::expandAll);
}

void MassDestribParamsPage::selectFolder()
{
    QString dir = QFileDialog::getExistingDirectory(this);
    ui->lineEdit->setText(dir);
}
