#ifndef ORACLEAUTHDLG_H
#define ORACLEAUTHDLG_H

#include <QDialog>
#include <QtSql>
#include "fmtlib_global.h"

namespace Ui {
class OracleAuthDlg;
}

class OracleTnsListModel;
class ConnectionInfo;
class RecentConnectionList;
class OdbcTableModel;
class OdbcTableModelDelegate;
class FMTLIBSHARED_EXPORT OracleAuthDlg : public QDialog
{
	Q_OBJECT

public:
    OracleAuthDlg(QWidget *parent = Q_NULLPTR);
    OracleAuthDlg(OracleTnsListModel *tnsmodel, QWidget *parent = Q_NULLPTR);
	~OracleAuthDlg();
	bool authirizate();
	void LoadPrefs();
    void SavePrefs(bool Append = true);
    ConnectionInfo *getConnectionInfo();

    static bool tryConnect(ConnectionInfo *pTmpInfo,
                           const QString &user,
                           const QString &pass,
                           const QString &dsn,
                           const QString &options,
                           QWidget *parent);
    static QString OptionsMapToOptions(const QMap<QString, QVariant> &map);
private:
    Ui::OracleAuthDlg *ui;

private slots:
    void acceptClicked();
    void on_connectionsTree_doubleClicked(const QModelIndex &index);
    void on_delRecentButton_clicked();
    void on_connectionsTree_clicked(const QModelIndex &index);
    void on_pushButton_accept_clicked();

private:
    QScopedPointer<RecentConnectionList> m_pRecentList;
    QScopedPointer<OdbcTableModel> m_DataSources;
    QScopedPointer<OdbcTableModelDelegate> m_m_DataSourceDelegate;
    ConnectionInfo *pTmpInfo;
};

#endif // ORACLEAUTHDLG_H
