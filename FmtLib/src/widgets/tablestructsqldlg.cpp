#include "tablestructsqldlg.h"
#include "ui_tablestructsqldlg.h"
#include "src/models/tableobjectsmodel.h"
#include "codeeditor/codeeditor.h"
#include "codeeditor/codehighlighter.h"

TableStructSqlDlg::TableStructSqlDlg(ConnectionInfo *connection, const QString &tableName, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::TableStructSqlDlg),
    m_pConnection(connection)
{
    ui->setupUi(this);

    m_pModel.reset(new TableObjectsModel(connection, tableName));
    ui->treeView->setModel(m_pModel.data());

    ToolApplyHighlighter(ui->plainTextEdit, HighlighterSql);

    ui->treeView->expandAll();
    connect(ui->treeView, &QTreeView::clicked, this, &TableStructSqlDlg::onTreeItemClicked);
}

TableStructSqlDlg::~TableStructSqlDlg()
{
    delete ui;
}

void TableStructSqlDlg::onTreeItemClicked(const QModelIndex &index)
{
    QString sql = m_pModel->getSqlForIndex(index);
    ui->plainTextEdit->setPlainText(sql);
}
