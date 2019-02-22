#ifndef TABLEMAPTOFMTWIDGET_H
#define TABLEMAPTOFMTWIDGET_H

#include <QWidget>
#include "fmttable.h"

namespace Ui {
class TableMapToFmtWidget;
}

class ColumnAlignedLayout;
class QTableView;
class QComboBox;
class TableMapToFmtWidget : public QWidget
{
    Q_OBJECT
public:
    explicit TableMapToFmtWidget(FmtSharedTablePtr table, QWidget *parent = nullptr);
    ~TableMapToFmtWidget();

    QTableView *tableWidget();
    QComboBox *columnFilter(int columnIndex);

private slots:
    void invalidateAlignedLayout();
    void onSectionCountChanged(int oldCount, int newCount);

private:
    Ui::TableMapToFmtWidget *ui;
    ColumnAlignedLayout *alignedLayout;
    FmtSharedTablePtr pTable;
};

#endif // TABLEMAPTOFMTWIDGET_H
