#ifndef TABLESTRUCTSQLDLG_H
#define TABLESTRUCTSQLDLG_H

#include <QDialog>

namespace Ui {
class TableStructSqlDlg;
}

class ConnectionInfo;
class TableObjectsModel;
class TableStructSqlDlg : public QDialog
{
    Q_OBJECT

public:
    explicit TableStructSqlDlg(ConnectionInfo *connection, const QString &tableName, QWidget *parent = nullptr);
    ~TableStructSqlDlg();

    QString getObjectsSql();

private slots:
    void saveSettings();
    void onTreeItemClicked(const QModelIndex &index);

private:
    QString wrapCreateTableExecuteImmediate(const QString &sqlScript);
    QString wrapIndexDdlInExecuteImmediate(const QString &indexDdl);
    QString wrapSequenceDdlInExecuteImmediate(const QString &sequenceDdl);
    QString wrapTriggerDdlInExecuteImmediate(const QString &triggerDdl);

    Ui::TableStructSqlDlg *ui;
    ConnectionInfo *m_pConnection;

    QScopedPointer<TableObjectsModel> m_pModel;
};

#endif // TABLESTRUCTSQLDLG_H
