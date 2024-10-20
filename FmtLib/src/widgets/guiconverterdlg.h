#ifndef GUICONVERTERDLG_H
#define GUICONVERTERDLG_H

#include <QDialog>

namespace Ui {
class GuiConverterDlg;
}

class RecentConnectionList;
class OdbcTableModel;
class OdbcTableModelDelegate;
class FilterRecentConnectionList;
class QSettings;
class GuiConverterDlg : public QDialog
{
    Q_OBJECT

public:
    explicit GuiConverterDlg(QSettings *settings, QWidget *parent = nullptr);
    ~GuiConverterDlg();

private slots:
    void on_connectionsTree_clicked(const QModelIndex &index);

private:
    Ui::GuiConverterDlg *ui;

    QScopedPointer<FilterRecentConnectionList> m_pRecentListFilter;
    QScopedPointer<RecentConnectionList> m_pRecentList;
    QScopedPointer<OdbcTableModel> m_DataSources;
    QScopedPointer<OdbcTableModelDelegate> m_m_DataSourceDelegate;
};

#endif // GUICONVERTERDLG_H
