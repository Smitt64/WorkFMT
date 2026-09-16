#include "createtablefromsqldlg.h"
#include "codeeditor/codeeditor.h"
#include "codeeditor/codehighlighter.h"
#include "fmtcore.h"

#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGridLayout>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QMessageBox>
#include <QDialogButtonBox>

CreateTableFromSqlDlg::CreateTableFromSqlDlg(QWidget *parent) :
    QDialog(parent)
{
    setWindowTitle(tr("Создать запись по коду create table"));
    setMinimumSize(700, 500);

    QVBoxLayout *mainLayout = new QVBoxLayout(this);

    // Поле ввода SQL-запроса с подсветкой синтаксиса
    m_pEditor = new CodeEditor(this);
    ToolApplyHighlighter(m_pEditor, HighlighterSql, FmtCodeTabStyle);
    m_pEditor->setPlaceholderText(tr("Вставьте запрос CREATE TABLE..."));
    m_pEditor->setMinimumHeight(200);

    mainLayout->addWidget(m_pEditor, 1);

    // Имя таблицы и комментарий
    QGridLayout *paramsLayout = new QGridLayout();
    paramsLayout->addWidget(new QLabel(tr("Название таблицы:"), this), 0, 0);
    m_pNameEdit = new QLineEdit(this);
    paramsLayout->addWidget(m_pNameEdit, 0, 1);

    paramsLayout->addWidget(new QLabel(tr("Комментарий:"), this), 1, 0);
    m_pCommentEdit = new QLineEdit(this);
    paramsLayout->addWidget(m_pCommentEdit, 1, 1);

    mainLayout->addLayout(paramsLayout);

    // Информация о количестве полей
    m_pFieldsLabel = new QLabel(this);
    mainLayout->addWidget(m_pFieldsLabel);

    // Кнопки
    QHBoxLayout *buttonsLayout = new QHBoxLayout();
    m_pParseButton = new QPushButton(tr("Распарсить"), this);
    connect(m_pParseButton, &QPushButton::clicked, this, &CreateTableFromSqlDlg::parseSql);
    buttonsLayout->addWidget(m_pParseButton);
    buttonsLayout->addStretch();

    QDialogButtonBox *buttons = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel, this);
    m_pOkButton = buttons->button(QDialogButtonBox::Ok);
    m_pOkButton->setText(tr("Создать"));
    m_pOkButton->setEnabled(false);
    connect(m_pOkButton, &QPushButton::clicked, this, &CreateTableFromSqlDlg::onOkClicked);
    connect(buttons, &QDialogButtonBox::rejected, this, &QDialog::reject);
    buttonsLayout->addWidget(buttons);

    mainLayout->addLayout(buttonsLayout);
}

CreateTableFromSqlDlg::~CreateTableFromSqlDlg()
{
}

CreateTableSqlResult CreateTableFromSqlDlg::result() const
{
    return m_Result;
}

QString CreateTableFromSqlDlg::tableName() const
{
    return m_pNameEdit->text().simplified();
}

QString CreateTableFromSqlDlg::tableComment() const
{
    return m_pCommentEdit->text().simplified();
}

void CreateTableFromSqlDlg::updateFieldsInfo(int count)
{
    if (count > 0)
        m_pFieldsLabel->setText(tr("Распознано полей: %1").arg(count));
    else
        m_pFieldsLabel->clear();
}

bool CreateTableFromSqlDlg::parseSql()
{
    const QString sql = m_pEditor->toPlainText();

    if (sql.trimmed().isEmpty())
    {
        QMessageBox::warning(this, QString(), tr("Введите запрос CREATE TABLE"));
        m_pOkButton->setEnabled(false);
        return false;
    }

    CreateTableSqlResult parsed;
    if (!CreateTableSqlParser::parse(sql, parsed))
    {
        QMessageBox::critical(this, QString(), tr("Ошибка разбора запроса: %1").arg(parsed.error));
        m_pOkButton->setEnabled(false);
        updateFieldsInfo(0);
        return false;
    }

    m_Result = parsed;

    // Имя таблицы и комментарий можно скорректировать вручную
    if (m_pNameEdit->text().trimmed().isEmpty() || !m_pNameEdit->isModified())
        m_pNameEdit->setText(m_Result.tableName);

    if (m_pCommentEdit->text().trimmed().isEmpty() || !m_pCommentEdit->isModified())
        m_pCommentEdit->setText(m_Result.comment);

    updateFieldsInfo(m_Result.fields.count());
    m_pOkButton->setEnabled(true);
    return true;
}

void CreateTableFromSqlDlg::onOkClicked()
{
    if (!parseSql())
        return;

    if (tableName().isEmpty())
    {
        QMessageBox::warning(this, QString(), tr("Укажите название таблицы"));
        return;
    }

    accept();
}
