#include "comparefmtviewtab.h"
#include "ui_comparefmtviewtab.h"
#include "src/core/fmttablecomparemodel.h"
#include "widgets/treeheadervew/gridtableheaderview.h"
#include <QTreeView>
#include <QTableView>
#include <QHeaderView>

CompareFmtViewTab::CompareFmtViewTab(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::CompareFmtViewTab)
{
    ui->setupUi(this);

#if VIEW_MODE == VIEW_TREE_MODE
    m_pView = new QTreeView(this);
#else
    m_pView = new QTableView(this);
#endif
    setCentralWidget(m_pView);

    m_pModel = new FmtTableCompareModel(this);
    m_pView->setModel(m_pModel);

    GridTableHeaderView *header = new GridTableHeaderView(Qt::Horizontal, 2, m_pModel->columnCount(), this);

#if VIEW_MODE == VIEW_TREE_MODE
    m_pView->setHeader(header);
    m_pView->setRootIsDecorated(false);
#else
    m_pView->setHorizontalHeader(header);
    m_pView->verticalHeader()->setDefaultSectionSize(20);
    m_pView->setSelectionBehavior(QAbstractItemView::SelectRows);
#endif

    header->setCellLabel(0, FmtTableCompareModel::ColumnId, "№");
    header->setCellLabel(1, FmtTableCompareModel::ColumnMineName, "Имя столбца");
    header->setCellLabel(1, FmtTableCompareModel::ColumnMineType, "Тип");
    header->setCellLabel(1, FmtTableCompareModel::ColumnMineSize, "Размер");

    header->setCellLabel(1, FmtTableCompareModel::ColumnTheirsName, "Имя столбца");
    header->setCellLabel(1, FmtTableCompareModel::ColumnTheirsType, "Тип");
    header->setCellLabel(1, FmtTableCompareModel::ColumnTheirsSize, "Размер");

    header->setCellLabel(0, FmtTableCompareModel::ColumnMineName, "Mine");
    header->setCellLabel(0, FmtTableCompareModel::ColumnTheirsName, "Theirs");

    header->setSpan(0, 0, 2, 0); // №
    header->setSpan(0, FmtTableCompareModel::ColumnMineName, 0,
                    FmtTableCompareModel::ColumnMineSize - FmtTableCompareModel::ColumnId);
    header->setSpan(0, FmtTableCompareModel::ColumnTheirsName, 0,
                    FmtTableCompareModel::ColumnTheirsSize - FmtTableCompareModel::ColumnMineSize);

    header->resizeSection(0, 50);
}

CompareFmtViewTab::~CompareFmtViewTab()
{
    delete ui;
}

void CompareFmtViewTab::setLists(FmtTable *table, const QString &cppcstruct)
{
    m_pModel->setLists(table, cppcstruct);
}

void CompareFmtViewTab::setLists(FmtTable *table1, FmtTable *table2)
{
    m_pModel->setLists(table1, table2);
}
