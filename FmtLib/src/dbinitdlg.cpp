#include "dbinitdlg.h"
#include "ui_dbinitdlg.h"
#include "fmttable.h"
#include <QSharedPointer>

DbInitDlg::DbInitDlg(FmtTable *pTable, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DbInitDlg)
{
    ui->setupUi(this);

    if (pTable->isExists())
    {
        ui->createTable->setChecked(false);
        ui->createTable->setText(tr("Пересоздать таблицу"));
    }
}

DbInitDlg::~DbInitDlg()
{
    delete ui;
}

bool DbInitDlg::getCreateTableFlag()
{
    return ui->createTable->isChecked();
}

bool DbInitDlg::getCreteIndexFlag()
{
    return ui->createIndex->isChecked();
}
