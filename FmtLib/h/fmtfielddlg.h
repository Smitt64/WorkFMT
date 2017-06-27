#ifndef FMTFIELDDLG_H
#define FMTFIELDDLG_H

#include <QDialog>
#include "fmtlib_global.h"

namespace Ui {
class FmtFieldDlg;
}

class FmtFildsModel;
class QDataWidgetMapper;
class FMTLIBSHARED_EXPORT FmtFieldDlg : public QDialog
{
    Q_OBJECT

public:
    explicit FmtFieldDlg(QWidget *parent = 0);
    ~FmtFieldDlg();

    void setModel(FmtFildsModel *model);
    void setCurrentIndex(const int &index);

private slots:
    void currentIndexChanged(const int &index);
    void typeChanged(const int &index);

private:
    void ResetMaping();
    Ui::FmtFieldDlg *ui;
    QDataWidgetMapper *mapper;
};

#endif // FMTFIELDDLG_H
