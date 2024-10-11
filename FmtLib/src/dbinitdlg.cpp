#include "dbinitdlg.h"
#include "ui_dbinitdlg.h"
#include "fmttable.h"
#include "connectioninfo.h"
#include <QSharedPointer>

DbInitDlg::DbInitDlg(FmtTable *pTable, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DbInitDlg)
{
    ui->setupUi(this);

    ConnectionInfo *info = pTable->connection();

    if (pTable->isTemporary() && info->type() == ConnectionInfo::CON_POSTGRESQL)
    {
        ui->createTable->setText(tr("Cоздать запись в create_table_info"));
        ui->createTable->setChecked(true);
        ui->createTable->setEnabled(false);
        ui->createIndex->setEnabled(false);
        /*if (pTable->isExistsInDb())
        {
            ui->createTable->setChecked(false);
            ui->createTable->setText(tr("Пересоздать таблицу"));
        }*/
    }
    else
    {
        if (pTable->isExistsInDb())
        {
            ui->createTable->setChecked(false);
            ui->createTable->setText(tr("Пересоздать таблицу"));
        }
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
