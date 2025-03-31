#include "comparecppstructpage.h"
#include "ui_comparecppstructpage.h"
#include "codeeditor/codeeditor.h"
#include "codeeditor/codehighlighter.h"

bool isStructDeclaration(const QString &str)
{
    // Упрощенное регулярное выражение для проверки объявления структуры
    QRegularExpression regex(
        "^\\s*struct\\s+"                // начинается с struct и пробелов
        "(\\w+\\s*)?"                    // необязательное имя структуры
        "(\\s*:\\s*\\w+\\s*)?"           // необязательное наследование
        "\\s*\\{?\\s*"                   // необязательная открывающая фигурная скобка
        "(//.*|/\\*.*\\*/|\\s*)*$"      // возможные комментарии в конце
    );

    QRegularExpressionMatch match = regex.match(str);
    return match.hasMatch();
}

CompareCppStructPage::CompareCppStructPage(QWidget *parent) :
    QWizardPage(parent),
    ui(new Ui::CompareCppStructPage)
{
    ui->setupUi(this);
    ToolApplyHighlighter(ui->plainTextEdit, HighlighterCpp);

    registerField("struct", ui->plainTextEdit, "plainText");
    setTitle(tr("Объявление структуры"));

    connect(ui->plainTextEdit, &QPlainTextEdit::textChanged, this, &CompareCppStructPage::completeChanged);
}

CompareCppStructPage::~CompareCppStructPage()
{
    delete ui;
}

int CompareCppStructPage::nextId() const
{
    return -1;
}

bool CompareCppStructPage::isComplete() const
{
    return !ui->plainTextEdit->toPlainText().isEmpty();
}
