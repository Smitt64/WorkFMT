#include "stringlistdlg.h"
#include "ui_stringlistdlg.h"
#include <QPushButton>

StringListDlg::StringListDlg(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::StringListDlg)
{
    ui->setupUi(this);
    ui->listView->setModel(&model);

    QPushButton *btn = ui->buttonBox->button(QDialogButtonBox::Ok);
    btn->setEnabled(false);

    connect(ui->listView->selectionModel(), SIGNAL(selectionChanged(QItemSelection,QItemSelection)), SLOT(selectionChanged()));
    connect(ui->listView, SIGNAL(doubleClicked(QModelIndex)), SLOT(accept()));
}

StringListDlg::~StringListDlg()
{
    delete ui;
}

void StringListDlg::setList(const QStringList &lst)
{
    model.setStringList(lst);
}

QString StringListDlg::selected() const
{
    QString str;

    if (ui->listView->selectionModel()->hasSelection())
        str = model.data(ui->listView->selectionModel()->currentIndex(), Qt::DisplayRole).toString();

    return str;
}

void StringListDlg::selectionChanged()
{
    QPushButton *btn = ui->buttonBox->button(QDialogButtonBox::Ok);
    btn->setEnabled(ui->listView->selectionModel()->hasSelection());
}
