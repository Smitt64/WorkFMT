#include "selectfiltereddlg.h"
#include "ui_selectfiltereddlg.h"
#include <QSortFilterProxyModel>

SelectFilteredDlg::SelectFilteredDlg(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SelectFilteredDlg)
{
    ui->setupUi(this);
    connect(ui->lineEdit, SIGNAL(textChanged(QString)), SIGNAL(textChanged(QString)));
}

SelectFilteredDlg::~SelectFilteredDlg()
{
    delete ui;
}

void SelectFilteredDlg::setFilteredModel(QSortFilterProxyModel *model)
{
    ui->treeView->setModel(model);
}

void SelectFilteredDlg::setHidenColumns(const QList<int> &lst)
{
    for (int i = 0; i < ui->treeView->model()->columnCount(); i++)
        ui->treeView->showColumn(i);

    foreach (const int &i, lst) {
        ui->treeView->hideColumn(i);
    }
}

void SelectFilteredDlg::setColumnWidth(const int &col, const int &w)
{
    ui->treeView->setColumnWidth(col, w);
}

QDialogButtonBox *SelectFilteredDlg::dialogButtonBox()
{
    return ui->buttonBox;
}
