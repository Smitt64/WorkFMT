#include "richtexttoinsertresultpage.h"
#include "codeeditor/codeeditor.h"
#include "codeeditor/codehighlighter.h"
#include "richtexttoinsertwizard.h"
#include "richtexttoinsertrun.h"
#include <QPushButton>
#include <QVBoxLayout>
#include <QLabel>
#include <QThreadPool>
#include <QMessageBox>
#include <QApplication>
#include <QClipboard>

RichTextToInsertResultPage::RichTextToInsertResultPage(QWidget *parent) :
    QWizardPage(parent),
    m_pTextEdit(nullptr)
    , m_pStatusLabel(nullptr)
    , m_pCopyButton(nullptr)
    , m_pWorker(nullptr)
{
    setupUI();
}

RichTextToInsertResultPage::~RichTextToInsertResultPage()
{

}

void RichTextToInsertResultPage::setupUI()
{
    setTitle(tr("Результат генерации PL/SQL кода"));
    setSubTitle(tr("Сгенерированный код для вставки данных в таблицу"));

    QVBoxLayout *mainLayout = new QVBoxLayout(this);

    // Статусная метка
    m_pStatusLabel = new QLabel(tr("Подготовка к генерации кода..."), this);
    m_pStatusLabel->setAlignment(Qt::AlignCenter);
    mainLayout->addWidget(m_pStatusLabel);

    // Текстовое поле для отображения кода
    m_pTextEdit = new CodeEditor(this);
    m_pTextEdit->setReadOnly(true);
    m_pTextEdit->setPlaceholderText(tr("Здесь будет отображен сгенерированный PL/SQL код"));
    mainLayout->addWidget(m_pTextEdit);
    ToolApplyHighlighter(m_pTextEdit, HighlighterSql);

    // Кнопка копирования
    m_pCopyButton = new QPushButton(tr("Копировать в буфер обмена"), this);
    m_pCopyButton->setEnabled(false);
    //connect(m_pCopyButton, &QPushButton::clicked, this, &RichTextToInsertResultPage::onCopyToClipboard);
    mainLayout->addWidget(m_pCopyButton);
}

void RichTextToInsertResultPage::initializePage()
{
    m_pStatusLabel->setText(tr("Генерация PL/SQL кода..."));
    m_pTextEdit->clear();
    m_pCopyButton->setEnabled(false);
    m_PlsqlCode.clear();

    // Запускаем генерацию в отдельном потоке
    generatePlsqlCode();
}

void RichTextToInsertResultPage::generatePlsqlCode()
{
    // Получаем данные из wizard
    RichTextToInsertWizard *wizard = qobject_cast<RichTextToInsertWizard*>(this->wizard());
    if (!wizard)
    {
        m_pStatusLabel->setText(tr("Ошибка: не удалось получить мастер"));
        return;
    }

    QTextDocument *document = wizard->document();
    FmtTable *table = wizard->table();
    bool firstAsHeader = wizard->firstAsHeader();
    QMap<QString, QString> fieldMapping = wizard->fieldMapping(); // изменился тип

    if (!document || !table)
    {
        m_pStatusLabel->setText(tr("Ошибка: отсутствуют необходимые данные"));
        return;
    }

    // Создаем и настраиваем worker
    m_pWorker = new RichTextToInsertRun();
    m_pWorker->setData(document, table, firstAsHeader, fieldMapping);

    // Подключаем сигналы
    connect(m_pWorker, &RichTextToInsertRun::finished, this, &RichTextToInsertResultPage::onPlsqlGenerated);
    connect(m_pWorker, &QObject::destroyed, this, &RichTextToInsertResultPage::onGenerationFinished);

    // Запускаем в thread pool
    QThreadPool::globalInstance()->start(m_pWorker);
}

void RichTextToInsertResultPage::onGenerationFinished()
{
    // Опционально: можно добавить дополнительную обработку
}

void RichTextToInsertResultPage::onCopyToClipboard()
{
    if (m_PlsqlCode.isEmpty())
        return;

    QClipboard *clipboard = QApplication::clipboard();
    clipboard->setText(m_PlsqlCode);

    QMessageBox::information(this,
                             tr("Копирование завершено"),
                             tr("PL/SQL код успешно скопирован в буфер обмена"));
}

void RichTextToInsertResultPage::onPlsqlGenerated(const QString &plsqlCode)
{
    m_PlsqlCode = plsqlCode;

    if (m_PlsqlCode.isEmpty())
    {
        m_pStatusLabel->setText(tr("Ошибка: не удалось сгенерировать код"));
        m_pTextEdit->setPlainText(tr("Не удалось сгенерировать PL/SQL код. Проверьте исходные данные."));
    }
    else
    {
        m_pStatusLabel->setText(tr("Генерация завершена успешно"));
        m_pTextEdit->setPlainText(m_PlsqlCode);
        m_pCopyButton->setEnabled(true);

        // Прокручиваем к началу
        QTextCursor cursor = m_pTextEdit->textCursor();
        cursor.setPosition(0);
        m_pTextEdit->setTextCursor(cursor);
    }

    emit completeChanged();
}
