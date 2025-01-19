#ifndef TABLESSELECTWIDGET_H
#define TABLESSELECTWIDGET_H

#include <QtWidgets>
#include <functional>
#include "fmtlib_global.h"

class TablesDockWidget;
class FmtTablesModel;
class ConnectionInfo;
/*typedef bool(*AddFunc)(const QString&);
typedef bool(*RemFunc)(const QString&);*/
typedef std::function<bool(const QString&)> TablesSelectAddFunc;
typedef std::function<bool(const QString&)> TablesSelectRemFunc;
class FMTLIBSHARED_EXPORT TablesSelectWidget : public QWidget
{
    Q_OBJECT
public:
    explicit TablesSelectWidget(ConnectionInfo *connection, QWidget *parent = Q_NULLPTR);
    void setAddFunc(TablesSelectAddFunc func);
    void setRemFunc(TablesSelectRemFunc func);

    void beginAddTables(const QString &msg, const int &min, const int &max);
    bool userAddTable(const QString &table);
    void endAddTables();

    void clearSelected();

    int tablesCount() const;
    QString tableName(const int &index) const;

signals:
    void tableAdded(const QString &name);

private slots:
    void addButtonPressed();
    void addAllButtonPressed();
    void removeButtonPressed();
    void removeAllButtonPressed();
    void doubleAddClicked(const QModelIndex &index);
    void doubleRemoveClicked(const QModelIndex &index);

protected:
    virtual bool eventFilter(QObject *obj, QEvent *event) override;

private:
    void CopyToDstList(const QModelIndex &index);
    QHBoxLayout *horizontalLayout;
    TablesDockWidget *sourceList;
    QVBoxLayout *verticalLayout;
    QSpacerItem *verticalSpacer;
    QPushButton *addButton, *addAllButton;
    QPushButton *removeButton, *removeAllButton;
    QSpacerItem *verticalSpacer_2;
    QListView *dstListView;

    QFrame *hLine;
    QStandardItemModel *pDstModel;
    FmtTablesModel *pSourceModel;

    TablesSelectAddFunc pAddFunc;
    TablesSelectRemFunc pRemFunc;

    QProgressDialog *pUserAddDlg;
    int m_UserAddIndex;
};

#endif // TABLESSELECTWIDGET_H
