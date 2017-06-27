#include "aboutdlg.h"
#include "ui_aboutdlg.h"
#include "fmtcore.h"
#include <Windows.h>

AboutDlg::AboutDlg(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AboutDlg)
{
    ui->setupUi(this);
    ui->label->setPixmap(QPixmap(":/icon"));

    ui->labelBased->setText(tr("Основан на Qt %1").arg(QT_VERSION_STR));
    ui->textBrowser->setSource(QUrl("qrc:/about.html"));
    PutVersion();
}

AboutDlg::~AboutDlg()
{
    delete ui;
}

void AboutDlg::PutVersion()
{
    ui->labelVersion->setText(tr("Версия: %1").arg(GetVersionNumberString()));
}
