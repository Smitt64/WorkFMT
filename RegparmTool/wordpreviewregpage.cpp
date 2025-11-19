#include "wordpreviewregpage.h"
#include "ui_wordpreviewregpage.h"
#include "rsl/extractinfoexecutor.h"
#include "regparmwizard.h"

WordPreviewRegPage::WordPreviewRegPage(QWidget *parent) :
    QWizardPage(parent),
    ui(new Ui::WordPreviewRegPage)
{
    ui->setupUi(this);
}

WordPreviewRegPage::~WordPreviewRegPage()
{
    delete ui;
}

void WordPreviewRegPage::initializePage()
{
    RegParmWizard *wzrd = (RegParmWizard*)wizard();

    ExtractInfoExecutor executor;
    executor.setDocument(wzrd->wordContentDocument());
    executor.setDebugMacroFlag(true);
    executor.playRep("extract_info.mac");
}
