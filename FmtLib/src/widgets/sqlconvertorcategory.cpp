#include "sqlconvertorcategory.h"
#include "codeeditor/codeeditor.h"
#include "codeeditor/codehighlighter.h"
#include "toolsruntime.h"
#include "fmtcore.h"
#include "toolsqlconverter.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QStyle>
#include <QIcon>
#include <QToolButton>
#include <QRegularExpression>
#include <QDateTime>

SqlConvertorCategory::SqlConvertorCategory(QWidget *parent)
    : CategoryContentWidget(parent)
    , m_leftPanelContent(nullptr)
    , m_buttonContainer(nullptr)
    , m_convertButton(nullptr)
    , m_clearLogButton(nullptr)
    , m_usernameLabel(nullptr)
    , m_usernameEdit(nullptr)
    , m_mainSplitter(nullptr)
    , m_editorSplitter(nullptr)
    , m_sourceEditor(nullptr)
    , m_resultEditor(nullptr)
    , m_logEditor(nullptr)
{
    // Устанавливаем тип LayoutLeftPanel
    setLayoutType(LayoutLeftPanel);

    // Настраиваем левую панель
    setupLeftPanel();

    // Настраиваем правую панель
    setupRightPanel();
}

SqlConvertorCategory::~SqlConvertorCategory()
{
}

void SqlConvertorCategory::setupLeftPanel()
{
    // Создаем основной виджет для левой панели
    m_leftPanelContent = new QWidget();
    QVBoxLayout *mainLeftLayout = new QVBoxLayout(m_leftPanelContent);
    mainLeftLayout->setContentsMargins(10, 10, 10, 10);

    // Контейнер для кнопок (теперь горизонтальный)
    setupButtons();
    mainLeftLayout->addWidget(m_buttonContainer);

    // Добавляем разделитель
    QFrame *line = new QFrame();
    line->setFrameShape(QFrame::HLine);
    line->setFrameShadow(QFrame::Sunken);
    mainLeftLayout->addWidget(line);

    // Поле ввода пользователя
    QWidget *userContainer = new QWidget();
    QVBoxLayout *userLayout = new QVBoxLayout(userContainer);
    userLayout->setContentsMargins(0, 5, 0, 0);
    userLayout->setSpacing(5);

    m_usernameLabel = new QLabel("Пользователь:");
    m_usernameEdit = new QLineEdit();
    m_usernameEdit->setPlaceholderText("Введите имя пользователя");

    userLayout->addWidget(m_usernameLabel);
    userLayout->addWidget(m_usernameEdit);
    mainLeftLayout->addWidget(userContainer);

    // Добавляем растягивание, чтобы контент прижимался к верху
    mainLeftLayout->addStretch();

    // Устанавливаем виджет левой панели с фиксированной шириной 280px
    setLeftPanelWidget(m_leftPanelContent, 280);
}

void SqlConvertorCategory::setupButtons()
{
    m_buttonContainer = new QWidget();
    QHBoxLayout *buttonLayout = new QHBoxLayout(m_buttonContainer);
    buttonLayout->setContentsMargins(0, 0, 0, 10);
    buttonLayout->setSpacing(15);

    // Кнопка "Конвертировать"
    m_convertButton = new QToolButton();
    m_convertButton->setText("Конвертировать");
    m_convertButton->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
    m_convertButton->setIcon(QIcon::fromTheme("DataSourceTarget"));
    toolAddActionWithTooltip(m_convertButton, tr("Конвертировать SQL скрипт из Oracle синтаксиса в PostgreSQL"));

    m_convertButton->setIconSize(QSize(32, 32));
    m_convertButton->setFixedSize(100, 82);
    connect(m_convertButton, &QToolButton::clicked, this, &SqlConvertorCategory::onConvert);

    // Кнопка "Очистить лог"
    m_clearLogButton = new QToolButton();
    m_clearLogButton->setText("Очистить лог");
    m_clearLogButton->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
    m_clearLogButton->setIcon(QIcon::fromTheme("CleanData"));
    toolAddActionWithTooltip(m_clearLogButton, tr("Очистить содержимое окна вывода результатов выполнения макроса"));

    m_clearLogButton->setIconSize(QSize(32, 32));
    m_clearLogButton->setFixedSize(100, 82);
    connect(m_clearLogButton, &QToolButton::clicked, this, &SqlConvertorCategory::onClearLog);

    buttonLayout->addWidget(m_convertButton);
    buttonLayout->addWidget(m_clearLogButton);
    buttonLayout->addStretch();
}

void SqlConvertorCategory::setupRightPanel()
{
    // Создаем основной сплиттер (вертикальный)
    m_mainSplitter = new QSplitter(Qt::Vertical);

    // Внутренний сплиттер для редакторов (горизонтальный)
    m_editorSplitter = new QSplitter(Qt::Horizontal);

    // Создаем редакторы
    m_sourceEditor = new CodeEditor();
    m_resultEditor = new CodeEditor();
    m_logEditor = new QPlainTextEdit();

    // Применяем подсветку синтаксиса SQL
    ToolApplyHighlighter(m_sourceEditor, HighlighterSql, FmtCodeTabStyle);
    ToolApplyHighlighter(m_resultEditor, HighlighterSql, FmtCodeTabStyle);

    // Настраиваем редакторы
    m_sourceEditor->setPlaceholderText("Введите SQL-запрос...");
    m_resultEditor->setPlaceholderText("Результат конвертации...");
    m_resultEditor->setReadOnly(true);
    m_logEditor->setReadOnly(true);
    m_logEditor->setPlaceholderText("Лог операций...");

    // Добавляем редакторы во внутренний сплиттер
    m_editorSplitter->addWidget(m_sourceEditor);
    m_editorSplitter->addWidget(m_resultEditor);

    // Устанавливаем пропорции для редакторов (50% / 50%)
    m_editorSplitter->setStretchFactor(0, 1);
    m_editorSplitter->setStretchFactor(1, 1);

    // Добавляем внутренний сплиттер и лог в основной сплиттер
    m_mainSplitter->addWidget(m_editorSplitter);
    m_mainSplitter->addWidget(m_logEditor);

    // Настраиваем пропорции (70% для редакторов, 30% для лога)
    m_mainSplitter->setStretchFactor(0, 7);
    m_mainSplitter->setStretchFactor(1, 3);

    // Устанавливаем виджет детальной области
    setDetailWidget(m_mainSplitter);
}

void SqlConvertorCategory::applyButtonStyle(QToolButton *button)
{
    if (!button)
        return;

    QString styleSheet =
        "QToolButton {"
        "    background-color: transparent;"
        "    border: 1px solid #CCCCCC;"
        "    border-radius: 6px;"
        "    padding: 8px;"
        "    color: #333333;"
        "    font-weight: bold;"
        "}"
        "QToolButton:hover {"
        "    background-color: #E6F3EA;"
        "    border: 1px solid #217346;"
        "}"
        "QToolButton:pressed {"
        "    background-color: #C8E6D9;"
        "}"
        "QToolButton::icon {"
        "    margin-bottom: 5px;"
        "}"
        "QToolButton:focus {"
        "    outline: none;"
        "}";

    button->setStyleSheet(styleSheet);
    button->setCursor(Qt::PointingHandCursor);
}

void SqlConvertorCategory::addToLog(const QString &message)
{
    QString timestamp = QDateTime::currentDateTime().toString("hh:mm:ss");
    m_logEditor->appendPlainText(QString("[%1] %2").arg(timestamp, message));
}

SqlConversionResult SqlConvertorCategory::performConversion(const QString &sql, const QString &userName)
{
    SqlConversionResult result;

    // Вызываем существующую функцию конвертации из toolsqlconverter.h
    result = convertSql(sql, userName);

    return result;
}

void SqlConvertorCategory::onConvert()
{
    QString sql = m_sourceEditor->toPlainText();
    QString userName = m_usernameEdit->text();

    if (sql.isEmpty()) {
        addToLog("Ошибка: Введите SQL-запрос для конвертации");
        return;
    }

    addToLog(QString("Начало конвертации. Пользователь: %1").arg(userName.isEmpty() ? "не указан" : userName));

    // Выполняем конвертацию
    SqlConversionResult result = performConversion(sql, userName);

    // Отображаем результат
    m_resultEditor->setPlainText(result.result);

    if (!result.tail.isEmpty()) {
        addToLog(result.tail);
    }

    if (result.error.isEmpty()) {
        addToLog("Конвертация выполнена успешно");
    } else {
        addToLog("Ошибка конвертации: " + result.error);

        /*QString str = formatSqlErrorMessage(result.result);
        addToLog(str);*/
    }

    emit conversionCompleted(result.result, result.error);
}

void SqlConvertorCategory::onClearLog()
{
    m_logEditor->clear();
    addToLog("Лог очищен");
}

void SqlConvertorCategory::setConversionResult(const QString &result, const QString &error)
{
    if (!error.isEmpty()) {
        m_resultEditor->setPlainText(result);
        addToLog("Ошибка конвертации: " + error);
    } else {
        m_resultEditor->setPlainText(result);
        addToLog("Конвертация выполнена успешно");
    }

    emit conversionCompleted(result, error);
}

// Методы доступа
QString SqlConvertorCategory::sourceSql() const
{
    return m_sourceEditor ? m_sourceEditor->toPlainText() : QString();
}

QString SqlConvertorCategory::resultSql() const
{
    return m_resultEditor ? m_resultEditor->toPlainText() : QString();
}

QString SqlConvertorCategory::logText() const
{
    return m_logEditor ? m_logEditor->toPlainText() : QString();
}

QString SqlConvertorCategory::userName() const
{
    return m_usernameEdit ? m_usernameEdit->text() : QString();
}

void SqlConvertorCategory::setSourceSql(const QString &sql)
{
    if (m_sourceEditor)
        m_sourceEditor->setPlainText(sql);
}

void SqlConvertorCategory::setResultSql(const QString &sql)
{
    if (m_resultEditor)
        m_resultEditor->setPlainText(sql);
}

void SqlConvertorCategory::setLogText(const QString &text)
{
    if (m_logEditor)
        m_logEditor->setPlainText(text);
}

void SqlConvertorCategory::setUserName(const QString &name)
{
    if (m_usernameEdit)
        m_usernameEdit->setText(name);
}

void SqlConvertorCategory::clearLog()
{
    if (m_logEditor)
        m_logEditor->clear();
}

void SqlConvertorCategory::clearAll()
{
    setSourceSql(QString());
    setResultSql(QString());
    clearLog();
    setUserName(QString());
}