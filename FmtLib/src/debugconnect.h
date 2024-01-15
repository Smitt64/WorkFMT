#ifndef DEBUGCONNECT_H
#define DEBUGCONNECT_H

#include <QDialog>

namespace Ui {
class DebugConnect;
}

class RecentConnectionList;
class OdbcTableModel;
class OdbcTableModelDelegate;
class DebugConnect : public QDialog
{
    Q_OBJECT

public:
    explicit DebugConnect(QWidget *parent = nullptr);
    ~DebugConnect();

private slots:
    void on_connectionsTree_clicked(const QModelIndex &index);
    void apply();

private:
    Ui::DebugConnect *ui;

    QScopedPointer<RecentConnectionList> m_pRecentList;
    QScopedPointer<OdbcTableModel> m_DataSources;
    QScopedPointer<OdbcTableModelDelegate> m_m_DataSourceDelegate;
};

#endif // DEBUGCONNECT_H
