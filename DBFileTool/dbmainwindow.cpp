#include "dbmainwindow.h"
#include "ui_dbmainwindow.h"
#include <oracletnslistmodel.h>
#include <QFileDialog>
#include <QProgressDialog>

DbMainWindow::DbMainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::DbMainWindow())
{
    pObj = new DBFileObject(this);
    ui->setupUi(this);

    pTextLog = new QPlainTextEdit(this);
    logColor = new LogHighlighter(pTextLog->document());
    pTextLog->setReadOnly(true);
    ui->tabWidget->addTab(pTextLog, tr("Сообщения"));

    ui->toolButton->setIcon(QIcon("://img/openfolderHS.png"));

    pTnsModel = new OracleTnsListModel(this);
    ui->serviceBox->setModel(pTnsModel);

    ui->listView->setModel(&dbtModel);

    ui->userEdit->setText(pObj->lastUserName());
    ui->passEdit->setText(pObj->lastPassword());
    ui->serviceBox->setCurrentText(pObj->lastService());
    ui->dirEdit->setText(pObj->exportDir());

    connect(ui->unloadBtn, SIGNAL(clicked(bool)), SLOT(unload()));
    connect(ui->addBtn, SIGNAL(clicked(bool)), SLOT(addTable()));
    connect(ui->lineEdit, SIGNAL(returnPressed()), SLOT(addTable()));
    connect(ui->toolButton, SIGNAL(clicked(bool)), SLOT(exportDir()));
    connect(pObj, SIGNAL(procMessage(QString)), pTextLog, SLOT(appendPlainText(QString)));
}

DbMainWindow::~DbMainWindow()
{
    delete ui;
}

void DbMainWindow::addTable()
{
    QString dbt = ui->lineEdit->text().simplified();

    if (dbt.isEmpty())
        return;

    if (dbtModel.match(dbtModel.index(0, 0), Qt::DisplayRole, dbt).size())
        return;

    int row = dbtModel.rowCount();
    dbtModel.insertRow(row);
    dbtModel.setData(dbtModel.index(row, 0), dbt, Qt::DisplayRole);

    ui->lineEdit->setText("");
}

void DbMainWindow::exportDir()
{
    QString dir = QFileDialog::getExistingDirectory(this, QString(), ui->dirEdit->text());

    if (dir.isEmpty())
        return;

    ui->dirEdit->setText(dir);
    pObj->setExportDir(dir);
}

void DbMainWindow::unload()
{
    int i = 0;
    pObj->setLastUserName(ui->userEdit->text());
    pObj->setLastPassword(ui->passEdit->text());
    pObj->setLastService(ui->serviceBox->currentText());
    pTextLog->clear();
    ui->tabWidget->setCurrentIndex(1);

    QStringList lst = dbtModel.stringList();
    QProgressDialog dlg(tr("Выгрузка таблицы"), tr("Прервать"), 0, lst.size(), this);
    connect(&dlg, SIGNAL(canceled()), pObj, SLOT(stop()));
    dlg.open();
    foreach (const QString &str, lst) {
        if (dlg.wasCanceled())
            break;
        else
        {
            dlg.setValue(i);
            dlg.setLabelText(tr("Выгрузка таблицы %1, %2 из %3")
                             .arg(str)
                             .arg(++i)
                             .arg(lst.size()));

            pObj->unload(ui->userEdit->text(),
                         ui->passEdit->text(),
                         ui->serviceBox->currentText(),
                         ui->dirEdit->text(), str);
        }
    }
    dlg.close();
}
