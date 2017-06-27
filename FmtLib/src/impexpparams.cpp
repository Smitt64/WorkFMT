#include "impexpparams.h"
#include "ui_impexpparams.h"
#include "fmtapplication.h"
#include <QFileDialog>

ImpExpParams::ImpExpParams(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ImpExpParams)
{
    ui->setupUi(this);
    FmtApplication *app = (FmtApplication*)qApp;
    m_pPrm = app->settings();

    m_pPrm->beginGroup("FmtXml");
    ui->lineEdit->setText(m_pPrm->value("ExePath", "fmtxml/fmtxml.exe").toString());
    ui->comboBox->setCurrentText(m_pPrm->value("CodePage", "utf8").toString());
    ui->checkBox->setChecked(m_pPrm->value("NoXsd", false).toBool());
    m_pPrm->endGroup();

    connect(ui->toolPath, SIGNAL(clicked(bool)), SLOT(OpenFmtXml()));
    connect(this, SIGNAL(accepted()), SLOT(OnAccept()));
}

ImpExpParams::~ImpExpParams()
{
    delete ui;
}

void ImpExpParams::OpenFmtXml()
{
    QString path = QFileDialog::getOpenFileName(this, tr("Открыть fmtxml.exe"), "fmtxml/", "FmtXml (fmtxml.exe)");

    if (path.isEmpty())
        return;

    ui->lineEdit->setText(path);
}

void ImpExpParams::OnAccept()
{
    m_pPrm->beginGroup("FmtXml");
    m_pPrm->setValue("ExePath", ui->lineEdit->text());
    m_pPrm->setValue("CodePage", ui->comboBox->currentText());
    m_pPrm->setValue("NoXsd", ui->checkBox->isChecked());
    m_pPrm->endGroup();
}
