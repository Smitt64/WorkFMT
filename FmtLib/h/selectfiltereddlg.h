#ifndef SELECTFILTEREDDLG_H
#define SELECTFILTEREDDLG_H

#include <QDialog>

namespace Ui {
class SelectFilteredDlg;
}

class QSortFilterProxyModel;
class QDialogButtonBox;
class SelectFilteredDlg : public QDialog
{
    Q_OBJECT

public:
    explicit SelectFilteredDlg(QWidget *parent = 0);
    virtual ~SelectFilteredDlg();

    void setFilteredModel(QSortFilterProxyModel *model);
    void setHidenColumns(const QList<int> &lst);
    void setColumnWidth(const int &col, const int &w);

    QDialogButtonBox *dialogButtonBox();

signals:
    void textChanged(QString);

private:
    Ui::SelectFilteredDlg *ui;
};

#endif // SELECTFILTEREDDLG_H
