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
    connect(ui->listView, SIGNAL(activated(QModelIndex)), SIGNAL(completeChanged()));
}

MassOpSelectOperationPage::~MassOpSelectOperationPage()
{
    delete ui;
}

bool MassOpSelectOperationPage::isComplete() const
{
    return ui->listView->selectionModel()->hasSelection();
}
