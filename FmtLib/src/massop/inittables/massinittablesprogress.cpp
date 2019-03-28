#include "massinittablesprogress.h"
#include "ui_massinittablesprogress.h"
#include "errordlg.h"
#include "fmterrors.h"

MassInitTablesProgress::MassInitTablesProgress(QWidget *parent) :
    QWizardPage(parent),
    ui(new Ui::MassInitTablesProgress)
{
    ui->setupUi(this);
    setTitle(tr("Инициализация"));
    pErrors = new FmtErrors(this);
    pErrDlg = new ErrorDlg(ErrorDlg::mode_Widget);
    pErrDlg->setErrors(pErrors);
    ui->verticalLayout->insertWidget(0, pErrDlg);
}

MassInitTablesProgress::~MassInitTablesProgress()
{
    delete ui;
}
