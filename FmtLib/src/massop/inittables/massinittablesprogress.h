#ifndef MASSINITTABLESPROGRESS_H
#define MASSINITTABLESPROGRESS_H

#include <QWizardPage>

namespace Ui {
class MassInitTablesProgress;
}

class ErrorDlg;
class FmtErrors;
class MassInitTablesProgress : public QWizardPage
{
    Q_OBJECT

public:
    explicit MassInitTablesProgress(QWidget *parent = nullptr);
    ~MassInitTablesProgress();

private:
    Ui::MassInitTablesProgress *ui;
    FmtErrors *pErrors;
    ErrorDlg *pErrDlg;
};

#endif // MASSINITTABLESPROGRESS_H
