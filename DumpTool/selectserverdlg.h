#ifndef SELECTSERVERDLG_H
#define SELECTSERVERDLG_H

#include <QDialog>
#include <QSortFilterProxyModel>

namespace Ui {
class SelectServerDlg;
}

class DsnFilterModel : public QSortFilterProxyModel
{
public:
    DsnFilterModel(QObject *parent = nullptr);
    virtual ~DsnFilterModel();

protected:
    virtual bool filterAcceptsRow(int source_row, const QModelIndex &source_parent) const;
};

class OdbcTableModel;
class OdbcTableModelDelegate;
class SelectServerDlg : public QDialog
{
    Q_OBJECT

public:
    explicit SelectServerDlg(QWidget *parent = nullptr);
    ~SelectServerDlg();

    const QString &getUserName() const;
    const QString &getHost() const;
    const QString &getDatabase() const;
    const quint32 &getPort() const;

private slots:
    void on_listView_clicked(const QModelIndex &index);

    void on_listView_doubleClicked(const QModelIndex &index);

private:
    Ui::SelectServerDlg *ui;

    DsnFilterModel *m_pFilter;
    OdbcTableModel *m_pDsnModel;
    OdbcTableModelDelegate *m_DataSourceDelegate;

    QString m_UserName, m_Host, m_Database;
    quint32 m_Port;
};

#endif // SELECTSERVERDLG_H
