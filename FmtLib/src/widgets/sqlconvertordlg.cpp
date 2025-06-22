#include "sqlconvertordlg.h"
#include "ui_sqlconvertordlg.h"
#include "codeeditor/codeeditor.h"
#include "codeeditor/codehighlighter.h"
#include "toolsqlconverter.h"
#include <QSplitter>
#include <QPushButton>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QLineEdit>

SqlConvertorDlg::SqlConvertorDlg(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SqlConvertorDlg)
{
    ui->setupUi(this);
    setMinimumSize(800, 600);

    QVBoxLayout *mainLayout = new QVBoxLayout(this);

    // Горизонтальный layout для кнопок
    QHBoxLayout *buttonLayout = new QHBoxLayout();
    QPushButton *btnConvert = new QPushButton("Конвертировать", this);
    QPushButton *btnClearLog = new QPushButton("Очистить лог", this);

    m_usernameLabel = new QLabel("Пользователь:", this);
    m_usernameEdit = new QLineEdit(this);
    m_usernameEdit->setPlaceholderText("Введите имя пользователя");
    m_usernameEdit->setMaximumWidth(200);  // Ограничиваем ширину поля

    buttonLayout->addWidget(btnConvert);
    buttonLayout->addWidget(btnClearLog);
    buttonLayout->addStretch();  // Растягиваем оставшееся пространство
    buttonLayout->addWidget(m_usernameLabel);
    buttonLayout->addWidget(m_usernameEdit);

    // Основной сплиттер (вертикальный)
    m_mainSplitter = new QSplitter(Qt::Vertical, this);

    // Внутренний сплиттер для редакторов (горизонтальный)
    m_editorSplitter = new QSplitter(Qt::Horizontal, this);

    // Создаем редакторы
    m_pSource = new CodeEditor(this);
    m_pResult = new CodeEditor(this);
    m_pLog = new QPlainTextEdit(this);

    ToolApplyHighlighter(m_pSource, HighlighterSql);
    ToolApplyHighlighter(m_pResult, HighlighterSql);

    // Настраиваем редакторы
    m_pSource->setPlaceholderText("Введите SQL-запрос...");
    m_pResult->setPlaceholderText("Результат конвертации...");
    m_pLog->setReadOnly(true);

    // Добавляем редакторы во внутренний сплиттер
    m_editorSplitter->addWidget(m_pSource);
    m_editorSplitter->addWidget(m_pResult);

    // Добавляем внутренний сплиттер и лог в основной сплиттер
    m_mainSplitter->addWidget(m_editorSplitter);
    m_mainSplitter->addWidget(m_pLog);

    // Настраиваем пропорции (например, 70% для редакторов, 30% для лога)
    m_mainSplitter->setStretchFactor(0, 7);
    m_mainSplitter->setStretchFactor(1, 3);

    // Добавляем кнопки и сплиттер в основной layout
    mainLayout->addLayout(buttonLayout);
    mainLayout->addWidget(m_mainSplitter);

    connect(btnConvert, &QPushButton::clicked, this, &SqlConvertorDlg::onConvertClicked);
    connect(btnClearLog, &QPushButton::clicked, this, &SqlConvertorDlg::onClearLogClicked);
}

SqlConvertorDlg::~SqlConvertorDlg()
{
    delete ui;
}

void SqlConvertorDlg::onConvertClicked()
{
    // TODO: Логика конвертации SQL
    QString sql = m_pSource->toPlainText();
    SqlConversionResult result = convertSql(sql, m_usernameEdit->text());

    m_pResult->setPlainText(result.result);

    if (result.error.isEmpty())
        m_pLog->appendPlainText("Конвертация выполнена успешно");
    else
    {
        m_pLog->appendPlainText("Ошибка конвертации: " + result.error);

        if (!result.tail.isEmpty())
            m_pLog->appendPlainText("Остался нераспознанный скрипт: " + result.tail);
    }
}

void SqlConvertorDlg::onClearLogClicked()
{
    m_pLog->clear();
}

void SqlConvertorDlg::setUserName(const QString &name)
{
    m_usernameEdit->setText(name);
}
