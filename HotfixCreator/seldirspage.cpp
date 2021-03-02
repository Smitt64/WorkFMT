#include "seldirspage.h"
#include "ui_seldirspage.h"

SelDirsPage::SelDirsPage(QWidget *parent) :
    QWizardPage(parent),
    ui(new Ui::SelDirsPage)
{
    ui->setupUi(this);

    m_pSourceWrp = new SelectDirWrp(ui->sourceEdit, this);
    m_pHotfixWrp = new SelectDirWrp(ui->hotfixEdit, this);

    registerField("sourceEdit*", ui->sourceEdit);
    registerField("hotfixEdit*", ui->hotfixEdit);

    setSubTitle(tr("Рабочие каталоги"));

#ifdef QT_DEBUG
    ui->sourceEdit->setText("d:/svn/UranRSBankV6/Renewal_2031_60");
    ui->hotfixEdit->setText("D:/Work/WorkFMT/HotfixCreator/release");
#endif

    connect(ui->sourceButton, &QToolButton::clicked, m_pSourceWrp, &SelectDirWrp::selectDirSlot);
    connect(ui->hotfixButton, &QToolButton::clicked, m_pHotfixWrp, &SelectDirWrp::selectDirSlot);
}

SelDirsPage::~SelDirsPage()
{
    delete ui;
}

bool SelDirsPage::isComplete() const
{
    return !ui->sourceEdit->text().isEmpty() && !ui->hotfixEdit->text().isEmpty();
}
