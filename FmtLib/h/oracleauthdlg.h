#ifndef ORACLEAUTHDLG_H
#define ORACLEAUTHDLG_H

#include <QDialog>
#include <QtSql>
//#include "ui_oracleauthdlg.h"
#include "fmtlib_global.h"

typedef struct
{
    QString dsn;
    QString user;
    QString pass;
} RecentList;
Q_DECLARE_METATYPE(RecentList)

namespace Ui {
class OracleAuthDlg;
}

class OracleTnsListModel;
class ConnectionInfo;
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

    QString getConnectionName() { return connectionname; }
    QString getConnectionSheme();

    ConnectionInfo *getConnectionInfo();

    static bool loadRecentList(QStringList &last, QList<RecentList> &recent);
    static QString OraRecentConnectionToolTip(const RecentList &item);
    static QString OraMakeConnectionName(const RecentList &item);
    /*static QSqlDatabase OraCreateConnection(const QString &username,
                                            const QString &passw,
                                            const QString &host,
                                            const QString &service,
                                            const QString &sheme, const QString &DSN,
                                            const int &port,
                                            ConnectionInfo **info);

    static QSqlDatabase OraCreateConnection(const QString &username,
                                            const QString &passw,
                                            const QString &DSN,
                                            ConnectionInfo **info,
                                            OracleTnsListModel *tnsmodel = Q_NULLPTR);*/
private:
    Ui::OracleAuthDlg *ui;
private slots:
	void on_pushButton_accept_clicked();
    void on_tableWidget_doubleClicked(const QModelIndex &index);
    //void on_comboBox_currentIndexChanged(int index);

    void on_delRecentButton_clicked();

private:
    //void PumpPrefs(const int &id);
    OracleTnsListModel *model;
    QString connectionname, sheme;
    QString sConfigOraFilePath;
    QList<RecentList> recentList;

    ConnectionInfo *pTmpInfo;
};

#endif // ORACLEAUTHDLG_H
