#include "textwizardpage.h"
#include "ui_textwizardpage.h"

TextWizardPage::TextWizardPage(QWidget *parent) :
    QWizardPage(parent),
    ui(new Ui::TextWizardPage)
{
    ui->setupUi(this);

    registerField("Files", ui->plainTextEdit, "plainText");
}

TextWizardPage::~TextWizardPage()
{
    delete ui;
}
