#ifndef COMPAREFMTVIEWTAB_H
#define COMPAREFMTVIEWTAB_H

#include <QMainWindow>
#include "src/core/fmttablecomparemodel.h"

#define VIEW_TABLE_MODE 1
#define VIEW_TREE_MODE 2

#define VIEW_MODE VIEW_TABLE_MODE

namespace Ui {
class CompareFmtViewTab;
}

class FmtTable;
class QTreeView;
class QTableView;
class FmtTableCompareModel;
class CompareFmtViewTab : public QMainWindow
{
    Q_OBJECT

public:
    explicit CompareFmtViewTab(QWidget *parent = nullptr);
    ~CompareFmtViewTab();

    void setLists(FmtTable *table, const QString &cppcstruct);
    void setLists(FmtTable *table1, FmtTable *table2);
    void setLists(FmtTable *table1, const FmtFldElementVector &table2);

private:
    Ui::CompareFmtViewTab *ui;

#if VIEW_MODE == VIEW_TREE_MODE
    QTreeView *m_pView;
#else
    QTableView *m_pView;
#endif

    FmtTableCompareModel *m_pModel;
};

#endif // COMPAREFMTVIEWTAB_H
