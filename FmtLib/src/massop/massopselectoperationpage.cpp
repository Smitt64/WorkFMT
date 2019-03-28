#include "massopselectoperationpage.h"
#include "ui_massopselectoperationpage.h"

MassOpSelectOperationPage::MassOpSelectOperationPage(QWidget *parent) :
    QWizardPage(parent),
    ui(new Ui::MassOpSelectOperationPage)
{
    ui->setupUi(this);
    pModel = new AbstractFactoryModel<MassOpInterface,QString>(&MassOpInterface::m_pMassOpInterfaceFactory, this);
    ui->listView->setModel(pModel);
    ui->listView->setModelColumn(AbstractFactoryModel<MassOpInterface,QString>::FieldAlias);
    connect(ui->listView, SIGNAL(clicked(QModelIndex)), SIGNAL(completeChanged()));
}

MassOpSelectOperationPage::~MassOpSelectOperationPage()
{
    delete ui;
}

bool MassOpSelectOperationPage::isComplete() const
{
    return ui->listView->selectionModel()->hasSelection();
}

QString MassOpSelectOperationPage::selectedItem() const
{
    return pModel->data(ui->listView->currentIndex(), Qt::DisplayRole).toString();
}

QString MassOpSelectOperationPage::selectedInterface() const
{
    QModelIndex index = ui->listView->currentIndex();
    QString name = pModel->data(pModel->index(index.row(), AbstractFactoryModel<MassOpInterface,QString>::FieldKey), Qt::DisplayRole).toString();
    return name;
}
