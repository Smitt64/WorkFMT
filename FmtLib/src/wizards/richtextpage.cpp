#include "richtextpage.h"
#include "ui_richtextpage.h"

RichTextPage::RichTextPage(QWidget *parent)
    : QWizardPage(parent)
    , ui(new Ui::RichTextPage)
{
    ui->setupUi(this);

    connect(ui->textEdit, &QTextEdit::textChanged, this, &RichTextPage::onTextChanged);
}

RichTextPage::~RichTextPage()
{
    delete ui;
}

void RichTextPage::setDocument(QTextDocument *document)
{
    ui->textEdit->setDocument(document);
}

bool RichTextPage::isComplete() const
{
    QString text = ui->textEdit->toPlainText().trimmed();

    if (text.isEmpty())
        return false;

    // Проверяем, что есть хотя бы один непробельный символ
    for (const QChar &ch : text)
    {
        if (!ch.isSpace())
            return true;
    }

    return false;
}

void RichTextPage::onTextChanged()
{
    emit completeChanged();
}
