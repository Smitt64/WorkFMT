#ifndef TABLEGROUPOPTIONS_H
#define TABLEGROUPOPTIONS_H

#include <QWidget>
#include <QVBoxLayout>
#include <optionsdlg/OptionsPage.h>

namespace Ui {
class TableGroupOptions;
}

class TablesSelectWidget;
class ConnectionInfo;
class TableGroupOptions : public OptionsPage
{
    Q_OBJECT

public:
    TableGroupOptions(ConnectionInfo *connection, QWidget *parent = nullptr);
    ~TableGroupOptions();

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
    Ui::TableGroupOptions *ui;

    QVBoxLayout *l;
    TablesSelectWidget *m_Tables;
};

#endif // TABLEGROUPOPTIONS_H
