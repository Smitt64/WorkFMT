#ifndef ERRORDLG_H
#define ERRORDLG_H

#include <QDialog>
#include <QStatusBar>
#include "fmterrors.h"
#include "fmtlib_global.h"

namespace Ui {
class ErrorDlg;
}

class QDialogButtonBox;
class FmtErrorFilterModel;
class FMTLIBSHARED_EXPORT ErrorDlg : public QDialog
{
    Q_OBJECT

public:
    enum
    {
        mode_Information,
        mode_MessageBox,
        mode_Widget
    };
    explicit ErrorDlg(const qint16 &mode, QWidget *parent = Q_NULLPTR);
    ~ErrorDlg();

    void setErrors(FmtErrors *e);
    void setMessage(const QString &msg);
    QDialogButtonBox *buttons();

private slots:
    void onActivated(const QModelIndex &index);
    void errorsCountChanged(const quint32 &value);
    void warningsCountChanged(const quint32 &value);
    void infoCountChanged(const quint32 &value);

private:
    Ui::ErrorDlg *ui;
    FmtErrorFilterModel *pFilterModel;
    FmtErrors *pErrors;
    qint16 m_Mode;
};

#endif // ERRORDLG_H
