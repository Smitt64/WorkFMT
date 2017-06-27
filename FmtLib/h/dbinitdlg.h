#ifndef DBINITDLG_H
#define DBINITDLG_H

#include <QDialog>
#include <QSharedPointer>
#include "fmtlib_global.h"

namespace Ui {
class DbInitDlg;
}

class FmtTable;
class FMTLIBSHARED_EXPORT DbInitDlg : public QDialog
{
    Q_OBJECT

public:
    explicit DbInitDlg(QSharedPointer<FmtTable> &pTable, QWidget *parent = 0);
    ~DbInitDlg();

    bool getCreateTableFlag();
    bool getCreteIndexFlag();

private:
    Ui::DbInitDlg *ui;
};

#endif // DBINITDLG_H
