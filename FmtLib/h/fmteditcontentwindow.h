#ifndef FMTEDITCONTENTWINDOW_H
#define FMTEDITCONTENTWINDOW_H

#include <QtWidgets>
#include "fmttable.h"
#include "fmteditcontentmodel.h"

class FmtEditContentWindow : public QMainWindow
{
    Q_OBJECT
public:
    explicit FmtEditContentWindow(FmtSharedTablePtr table, QWidget *parent = nullptr);

signals:

private slots:
    void OnAddRecord();
    void OnDeleteRecord();
    void OnDoubleClicked(const QModelIndex &index);
    void OnFilter();
    void OnImport();

private:
    void SetupColWidth();
    QTableView *pTableView;
    FmtSharedTablePtr pTable;
    FmtEditContentModel *pModel;

    QToolBar *pToolBar;
    QAction *pAddRecord, *pRemoveRecord, *pRefrash, *pFilter, *pImportAction;
};

#endif // FMTEDITCONTENTWINDOW_H
