#ifndef WINDOWSLISTDLG_H
#define WINDOWSLISTDLG_H

#include <QDialog>
#include <QMdiArea>
#include "fmtlib_global.h"

namespace Ui {
class WindowsListDlg;
}

class SubWindowsModel;
class FMTLIBSHARED_EXPORT WindowsListDlg : public QDialog
{
    Q_OBJECT

public:
    explicit WindowsListDlg(SubWindowsModel *Model, QMdiArea *mdi, QWidget *parent = 0);
    ~WindowsListDlg();

private slots:
    void CloseWindow();
    void ActivateWindow();

private:
    Ui::WindowsListDlg *ui;
    QPushButton *pSelect, *pClose;

    SubWindowsModel *pModel;
    QMdiArea *pMdi;
};

#endif // WINDOWSLISTDLG_H
