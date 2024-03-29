#ifndef PGEXPORTADDPRMDLG_H
#define PGEXPORTADDPRMDLG_H

#include <QDialog>

namespace Ui {
class PgExportAddPrmDlg;
}

class QStandardItemModel;
class PgExportAddPrmDlg : public QDialog
{
    Q_OBJECT

public:
    explicit PgExportAddPrmDlg(QWidget *parent = nullptr);
    ~PgExportAddPrmDlg();

    QStringList params() const;
    void setParams(const QString &params);

private:
    void initList();
    Ui::PgExportAddPrmDlg *ui;

    QStandardItemModel *m_pModel;
};

#endif // PGEXPORTADDPRMDLG_H
