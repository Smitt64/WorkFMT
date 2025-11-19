#include "wordcontentpage.h"
#include "ui_wordcontentpage.h"
#include <QTextDocument>

WordContentPage::WordContentPage(QWidget *parent) :
    QWizardPage(parent),
    ui(new Ui::WordContentPage)
{
    ui->setupUi(this);
}

WordContentPage::~WordContentPage()
{
    delete ui;
}

QTextDocument *WordContentPage::document()
{
    return ui->textEdit->document();
}
