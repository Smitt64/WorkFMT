#ifndef TABLESGROUPSDLG_H
#define TABLESGROUPSDLG_H

#include <QDialog>
#include <QtWidgets>
#include "fmtlib_global.h"

namespace Ui {
class TablesGroupsDlg;
}

class TablesSelectWidget;
class ConnectionInfo;
class FMTLIBSHARED_EXPORT TablesGroupsDlg : public QDialog
{
    Q_OBJECT

public:
    explicit TablesGroupsDlg(ConnectionInfo *connection, QWidget *parent = 0);
    ~TablesGroupsDlg();

private slots:
    void groupSelected(const int &index);

    void on_pushAddGroup_clicked();

    void on_pushRemoveGroup_clicked();

    void on_pushRename_clicked();

    void on_btnSave_clicked();

    void on_btLoad_clicked();

private:
    bool addTable(const QString &table);
    bool remTable(const QString &table);
    Ui::TablesGroupsDlg *ui;
    QVBoxLayout *l;
    TablesSelectWidget *m_Tables;
};

#endif // TABLESGROUPSDLG_H
