#ifndef TABLESDOCKWIDGET_H
#define TABLESDOCKWIDGET_H

#include <QMainWindow>
#include <QtWidgets>
#include "fmtlib_global.h"

class FmtTablesModel;
class TableComboAction;
class FMTLIBSHARED_EXPORT TablesDockWidget : public QMainWindow
{
    Q_OBJECT
public:
    explicit TablesDockWidget(QWidget *parent = Q_NULLPTR);
    void setModel(FmtTablesModel *model);

    QItemSelectionModel *selection();
    void setSelectionMode(const QAbstractItemView::SelectionMode &mode);
    QListView *listView();

    bool isFiltered();
    void updateList();

    void applyFilter(const QString &str);

private slots:
    void filterTriggered(bool checked);
    void filterTextChanged(const QString &text);
    void filterReturnPressed();
    void doubleClicked(const QModelIndex &index);
    void filterByGroupCombo(const int &id);
    void fromMuiltyLineText();
    void addToGroup();
    void createGroup();

signals:
    void tableDbClicked(quint32);

protected:
    bool eventFilter(QObject *watched, QEvent *event);

private:
    void AddTablesToGroup(const QString &GroupUid);
    TableComboAction *pFilterByGroupAction;
    QListView *pTables;
    QToolBar *pMainToolBar;

    QLineEdit *pFilterEdit;
    QMenu *pFilterMenu;
    QAction *pInitTableAction, *pRebuildOffsets, *pApplyFilterAction,
        *pFilterByGroup, *pMuiltyLineText, *pAddToGroupAction, *pCreateGroupAction;
    QLabel *pFilterLabel;

    QToolButton *pFilterAction;

    FmtTablesModel *pModel;
};

#endif // TABLESDOCKWIDGET_H
