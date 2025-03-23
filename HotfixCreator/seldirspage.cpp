#include "seldirspage.h"
#include "ui_seldirspage.h"
#include "hotfixwizard.h"

SelDirsPage::SelDirsPage(QWidget *parent) :
    QWizardPage(parent),
    ui(new Ui::SelDirsPage)
{
    ui->setupUi(this);

    HotfixWizard *wzrd = (HotfixWizard*)parent;
    m_pSourceWrp = new SelectDirWrp(ui->sourceEdit, wzrd->settings(), "SourceDir", this);
    m_pHotfixWrp = new SelectDirWrp(ui->hotfixEdit, wzrd->settings(), "HotfixDir", this);

    registerField("sourceEdit", ui->sourceEdit);
    registerField("hotfixEdit", ui->hotfixEdit);
    registerField("hotfixName", ui->hotfixName);
    registerField("checkOraPg", ui->checkOraPg);
    registerField("unpackDbExe", ui->unpackDbExe);

#ifdef _DEBUG
    ui->sourceEdit->setText("d:/svn/UranRSBankV6/Renewal_2031_91");
    ui->hotfixEdit->setText("D:/Work/WorkFMT/bin/hf_test");
    ui->hotfixName->setText("hf_89_test");
#endif

    ui->checkOraPg->setChecked(true);
    setTitle(tr("Рабочие каталоги"));

    connect(ui->sourceButton, &QToolButton::clicked, m_pSourceWrp, &SelectDirWrp::selectDirSlot);
    connect(ui->hotfixButton, &QToolButton::clicked, m_pHotfixWrp, &SelectDirWrp::selectDirSlot);
}

SelDirsPage::~SelDirsPage()
{
    delete ui;
    delete m_pSourceWrp;
    delete m_pHotfixWrp;
}

bool SelDirsPage::isComplete() const
{
    return !ui->sourceEdit->text().isEmpty() && !ui->hotfixEdit->text().isEmpty() && !ui->hotfixName->text().isEmpty();
}
