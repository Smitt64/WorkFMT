#include "dataselectdialog.h"
#include "ui_dataselectdialog.h"
#include <codeeditor/codeeditor.h>
#include <codeeditor/codehighlighter.h>
#include <QtSql>

DataSelectDialog::DataSelectDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DataSelectDialog),
    pdb(Q_NULLPTR)
{
    ui->setupUi(this);
    pEditor = new CodeEditor(this);
    ToolApplyHighlighter(pEditor, HighlighterSql);

    ui->tabWidget->addTab(pEditor, tr("Запрос"));

    pModel = new QSqlQueryModel(this);
    ui->tableView->setModel(pModel);
    ui->tableView->verticalHeader()->setDefaultSectionSize(25);

    pEditor->setEnabled(false);
    ui->toolButton->setEnabled(false);

    connect(ui->toolButton, SIGNAL(clicked(bool)), SLOT(OnRefrash()));
    connect(ui->tableView, SIGNAL(doubleClicked(QModelIndex)), SLOT(OnDoubleClicked(QModelIndex)));
}

DataSelectDialog::~DataSelectDialog()
{
    delete ui;
}

void DataSelectDialog::setQueryString(const QString &str, QSqlDatabase *db)
{
    pdb = db;
    QSqlQueryModel *model = qobject_cast<QSqlQueryModel*>(pModel);

    if (!model)
        return;

    model->setQuery(str, *pdb);
    pEditor->setPlainText(str);
    pEditor->setEnabled(true);
    ui->toolButton->setEnabled(true);
}

void DataSelectDialog::setQuery(const QString &tableName, const QString &fields, QSqlDatabase *db)
{
    QString query = QString("select distinct %1 from %2").arg(fields, tableName);
    setQueryString(query, db);
}

void DataSelectDialog::OnRefrash()
{
    setQueryString(pEditor->toPlainText(), (QSqlDatabase*)pdb);
    ui->tabWidget->setCurrentIndex(0);
}

QVariant DataSelectDialog::selectedValue(int role)
{
    QModelIndex index = ui->tableView->selectionModel()->currentIndex();
    return pModel->data(index, role);
}

void DataSelectDialog::OnDoubleClicked(const QModelIndex &index)
{
    Q_UNUSED(index)
    QDialog::accept();
}
