#include "selectconnectiondlg.h"
#include "connectionlistmodel.h"
#include "ui_selectconnectiondlg.h"

SelectConnectionDlg::SelectConnectionDlg(const QList<ConnectionInfo*> list, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SelectConnectionDlg)
{
    ui->setupUi(this);
    pModel = new ConnectionListModel(list);
    ui->listView->setModel(pModel);
}

SelectConnectionDlg::~SelectConnectionDlg()
{
    delete ui;
    delete pModel;
}

QList<ConnectionInfo*> SelectConnectionDlg::checkedItems() const
{
    return pModel->checkedItems();
}
