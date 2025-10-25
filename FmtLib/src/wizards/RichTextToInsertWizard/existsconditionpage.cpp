#include "existsconditionpage.h"
#include "src/wizards/RichTextToInsertWizard/richtexttoinsertwizard.h"
#include "ui_existsconditionpage.h"
#include "fmttable.h"
#include "fmtindex.h"
#include "fmtsegment.h"
#include "fmtfield.h"
#include "codeeditor/codehighlighter.h"
#include <QIcon>
#include <QButtonGroup>
#include <QCompleter>
#include <QPushButton>
#include <QMenu>
#include <QAction>
#include <QMessageBox>
#include <QRegularExpression>
#include <QRegularExpressionMatch>
#include <QHBoxLayout>
#include <QLabel>
#include <QVBoxLayout>
#include <functional>
#include <QTextTable>
#include <QTextFrame>
#include <QStandardItemModel>

// ============================================================================
// FmtIndexTreeItem - реализация методов
// ============================================================================

QVariant FmtIndexTreeItem::data(int column) const
{
    if (m_index)
    {
        if (column == 0)
            return m_index->name();
    }
    else if (m_segment)
    {
        if (column == 0)
            return m_segment->field() ? m_segment->field()->name() : QString();
    }
    return QVariant();
}

// ============================================================================
// FmtIndexTreeModel - модель дерева индексов
// ============================================================================

FmtIndexTreeModel::FmtIndexTreeModel(FmtTable *table, QObject *parent)
    : QAbstractItemModel(parent), m_table(table), m_selectedIndex(nullptr)
{
    m_rootItem = new FmtIndexTreeItem();
    setupModelData();
}

FmtIndexTreeModel::~FmtIndexTreeModel()
{
    delete m_rootItem;
}

void FmtIndexTreeModel::setupModelData()
{
    beginResetModel();

    delete m_rootItem;
    m_rootItem = new FmtIndexTreeItem();

    if (m_table)
    {
        for (int i = 0; i < m_table->indecesCount(); ++i)
        {
            FmtIndex *index = m_table->tableIndex(i);
            FmtIndexTreeItem *indexItem = new FmtIndexTreeItem(index, m_rootItem);
            m_rootItem->appendChild(indexItem);

            for (int j = 0; j < index->segmentsCount(); ++j)
            {
                FmtSegment *segment = index->segment(j);
                FmtIndexTreeItem *segmentItem = new FmtIndexTreeItem(segment, indexItem);
                indexItem->appendChild(segmentItem);
            }
        }
    }

    endResetModel();
}

QModelIndex FmtIndexTreeModel::index(int row, int column, const QModelIndex &parent) const
{
    if (!hasIndex(row, column, parent))
        return QModelIndex();

    FmtIndexTreeItem *parentItem;

    if (!parent.isValid())
        parentItem = m_rootItem;
    else
        parentItem = static_cast<FmtIndexTreeItem*>(parent.internalPointer());

    FmtIndexTreeItem *childItem = parentItem->child(row);
    if (childItem)
        return createIndex(row, column, childItem);

    return QModelIndex();
}

QModelIndex FmtIndexTreeModel::parent(const QModelIndex &child) const
{
    if (!child.isValid())
        return QModelIndex();

    FmtIndexTreeItem *childItem = static_cast<FmtIndexTreeItem*>(child.internalPointer());
    if (!childItem || childItem->isRoot())
        return QModelIndex();

    FmtIndexTreeItem *parentItem = childItem->parent();
    if (parentItem == m_rootItem || parentItem->isRoot())
        return QModelIndex();

    return createIndex(parentItem->row(), 0, parentItem);
}

int FmtIndexTreeModel::rowCount(const QModelIndex &parent) const
{
    if (parent.column() > 0)
        return 0;

    FmtIndexTreeItem *parentItem;
    if (!parent.isValid())
        parentItem = m_rootItem;
    else
        parentItem = static_cast<FmtIndexTreeItem*>(parent.internalPointer());

    return parentItem->childCount();
}

int FmtIndexTreeModel::columnCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return 1;
}

QVariant FmtIndexTreeModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();

    FmtIndexTreeItem *item = static_cast<FmtIndexTreeItem*>(index.internalPointer());
    if (!item || item->isRoot())
        return QVariant();

    if (role == Qt::DisplayRole || role == Qt::EditRole)
    {
        return item->data(index.column());
    }
    else if (role == Qt::DecorationRole)
    {
        if (item->isIndex())
        {
            if (item->index()->isPrimary())
                return QIcon(":/img/PrimaryKeyHS.png");
            else
                return QIcon(":/img/IndexHS.png");
        }
        else if (item->isSegment())
        {
            return QIcon(":/img/FieldHS.png");
        }
    }
    else if (role == Qt::CheckStateRole && item->isIndex())
    {
        return (item->index() == m_selectedIndex) ? Qt::Checked : Qt::Unchecked;
    }
    else if (role == Qt::UserRole)
    {
        if (item->isIndex()) return "index";
        if (item->isSegment()) return "segment";
    }

    return QVariant();
}

bool FmtIndexTreeModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if (!index.isValid() || role != Qt::CheckStateRole)
        return false;

    FmtIndexTreeItem *item = static_cast<FmtIndexTreeItem*>(index.internalPointer());
    if (!item || !item->isIndex())
        return false;

    if (value.toInt() == Qt::Checked)
    {
        // Снимаем выделение с предыдущего индекса
        if (m_selectedIndex && m_selectedIndex != item->index())
        {
            QModelIndex previousIndex = findIndexForIndex(m_selectedIndex);
            if (previousIndex.isValid())
            {
                m_selectedIndex = nullptr;
                emit dataChanged(previousIndex, previousIndex, {Qt::CheckStateRole});
            }
        }

        // Устанавливаем новый выбранный индекс
        m_selectedIndex = item->index();
        emit dataChanged(index, index, {Qt::CheckStateRole});
        emit selectionChanged(m_selectedIndex);
    }
    else
    {
        // Снимаем выделение
        if (m_selectedIndex == item->index())
        {
            m_selectedIndex = nullptr;
            emit dataChanged(index, index, {Qt::CheckStateRole});
            emit selectionChanged(nullptr);
        }
    }

    return true;
}

Qt::ItemFlags FmtIndexTreeModel::flags(const QModelIndex &index) const
{
    if (!index.isValid())
        return Qt::NoItemFlags;

    FmtIndexTreeItem *item = static_cast<FmtIndexTreeItem*>(index.internalPointer());
    if (!item || item->isRoot())
        return Qt::NoItemFlags;

    Qt::ItemFlags flags = Qt::ItemIsEnabled;

    if (item->isIndex())
    {
        flags |= Qt::ItemIsSelectable | Qt::ItemIsUserCheckable;
    }

    return flags;
}

FmtIndex* FmtIndexTreeModel::getIndex(const QModelIndex &index) const
{
    if (!index.isValid())
        return nullptr;

    FmtIndexTreeItem *item = static_cast<FmtIndexTreeItem*>(index.internalPointer());
    if (!item || item->isRoot())
        return nullptr;

    if (item->isIndex())
        return item->index();

    if (item->isSegment() && item->parent() && !item->parent()->isRoot())
        return item->parent()->index();

    return nullptr;
}

bool FmtIndexTreeModel::isIndex(const QModelIndex &index) const
{
    if (!index.isValid())
        return false;

    FmtIndexTreeItem *item = static_cast<FmtIndexTreeItem*>(index.internalPointer());
    return item && item->isIndex();
}

bool FmtIndexTreeModel::isSegment(const QModelIndex &index) const
{
    if (!index.isValid())
        return false;

    FmtIndexTreeItem *item = static_cast<FmtIndexTreeItem*>(index.internalPointer());
    return item && item->isSegment();
}

FmtIndexTreeItem *FmtIndexTreeModel::getItem(const QModelIndex &index) const
{
    if (!index.isValid())
        return m_rootItem;

    return static_cast<FmtIndexTreeItem*>(index.internalPointer());
}

QModelIndex FmtIndexTreeModel::findIndexForIndex(FmtIndex *index) const
{
    if (!index || !m_rootItem)
        return QModelIndex();

    std::function<QModelIndex(FmtIndexTreeItem*, int)> findRecursive;
    findRecursive = [&](FmtIndexTreeItem *parentItem, int row) -> QModelIndex {
        for (int i = 0; i < parentItem->childCount(); ++i)
        {
            FmtIndexTreeItem *childItem = parentItem->child(i);
            if (childItem->isIndex() && childItem->index() == index)
            {
                return createIndex(i, 0, childItem);
            }

            QModelIndex found = findRecursive(childItem, i);
            if (found.isValid())
                return found;
        }
        return QModelIndex();
    };

    return findRecursive(m_rootItem, 0);
}

FmtIndex* FmtIndexTreeModel::getSelectedIndex() const
{
    return m_selectedIndex;
}

QModelIndex FmtIndexTreeModel::getSelectedIndexModelIndex() const
{
    return findIndexForIndex(m_selectedIndex);
}

void FmtIndexTreeModel::setSelectedIndex(const QModelIndex &index)
{
    if (!index.isValid())
        return;

    FmtIndexTreeItem *item = static_cast<FmtIndexTreeItem*>(index.internalPointer());
    if (!item || !item->isIndex())
        return;

    setData(index, Qt::Checked, Qt::CheckStateRole);
}

void FmtIndexTreeModel::clearSelection()
{
    if (m_selectedIndex)
    {
        QModelIndex previousIndex = findIndexForIndex(m_selectedIndex);
        if (previousIndex.isValid())
        {
            m_selectedIndex = nullptr;
            emit dataChanged(previousIndex, previousIndex, {Qt::CheckStateRole});
            emit selectionChanged(nullptr);
        }
    }
}

// ============================================================================
// CustomConditionHelper - помощник для работы с условиями
// ============================================================================

CustomConditionHelper::CustomConditionHelper(FmtTable *table, QObject *parent)
    : QObject(parent), m_table(table)
{
}

QStringList CustomConditionHelper::getFieldNames() const
{
    QStringList fields;
    if (m_table)
    {
        for (int i = 0; i < m_table->fieldsCount(); ++i)
        {
            FmtField *field = m_table->field(i);
            if (field)
                fields << field->name();
        }
    }
    return fields;
}

QStringList CustomConditionHelper::getTableNames() const
{
    QStringList tables;
    if (m_table)
    {
        tables << m_table->name();
    }
    return tables;
}

QStringList CustomConditionHelper::getSqlKeywords() const
{
    return {
        "SELECT", "FROM", "WHERE", "AND", "OR", "NOT", "IN", "EXISTS",
        "LIKE", "BETWEEN", "IS NULL", "IS NOT NULL", "COUNT", "MAX", "MIN",
        "SUM", "AVG", "DISTINCT", "GROUP BY", "HAVING", "ORDER BY"
    };
}

QStringList CustomConditionHelper::getFunctions() const
{
    return {
        "TO_DATE", "TO_CHAR", "NVL", "COALESCE", "DECODE",
        "SUBSTR", "INSTR", "LENGTH", "UPPER", "LOWER",
        "TRIM", "LTRIM", "RTRIM", "REPLACE", "LPAD", "RPAD"
    };
}

QString CustomConditionHelper::generateTemplate(const QString &templateName) const
{
    QHash<QString, QString> templates = {
        {"basic_exists", "EXISTS (SELECT 1 FROM table_name WHERE field_name = 'value')"},
        {"count_check", "(SELECT COUNT(*) FROM table_name WHERE field_name = 'value') > 0"},
        {"single_field", "field_name = 'value'"},
        {"multiple_fields", "field1 = 'value1' AND field2 = 'value2'"},
        {"date_range", "field_name BETWEEN TO_DATE('01.01.2024', 'DD.MM.YYYY') AND TO_DATE('31.12.2024', 'DD.MM.YYYY')"},
        {"like_pattern", "field_name LIKE '%pattern%'"},
        {"in_list", "field_name IN ('value1', 'value2', 'value3')"},
        {"null_check", "field_name IS NOT NULL"}
    };

    return templates.value(templateName, "field_name = 'value'");
}

QString CustomConditionHelper::validateCondition(const QString &condition) const
{
    if (condition.trimmed().isEmpty())
        return "Условие не может быть пустым";

    QString upperCondition = condition.toUpper();

    // Проверка на наличие опасных выражений
    QVector<QString> dangerousPatterns = {
        "DROP TABLE", "DELETE FROM", "UPDATE ", "INSERT INTO",
        "CREATE TABLE", "ALTER TABLE", "EXECUTE", "EXEC ",
        "TRUNCATE TABLE", "GRANT ", "REVOKE "
    };

    for (const QString &pattern : dangerousPatterns)
    {
        if (upperCondition.contains(pattern))
        {
            return QString("Обнаружено потенциально опасное выражение: %1").arg(pattern);
        }
    }

    // Проверка на незакрытые скобки
    int openBrackets = condition.count('(');
    int closeBrackets = condition.count(')');
    if (openBrackets != closeBrackets)
    {
        return "Несовпадение количества открывающих и закрывающих скобок";
    }

    // Проверка на незакрытые кавычки (базовая)
    int singleQuotes = condition.count('\'');
    if (singleQuotes % 2 != 0)
    {
        return "Незакрытые кавычки в условии";
    }

    return ""; // Нет ошибок
}

// ============================================================================
// ExistsConditionPage - основная страница мастера
// ============================================================================

ExistsConditionPage::ExistsConditionPage(QWidget *parent)
    : QWizardPage(parent)
    , ui(new Ui::ExistsConditionPage),
    m_pModel(nullptr),
    m_pGroup(nullptr),
    m_conditionHelper(nullptr),
    m_completer(nullptr),
    m_isConditionValid(false),
    m_btnTemplates(nullptr),
    m_btnFields(nullptr),
    m_btnFunctions(nullptr),
    m_btnParameters(nullptr),
    m_btnValidate(nullptr),
    m_hintLabel(nullptr)
{
    ui->setupUi(this);
    setTitle(tr("Обновить или вставить?"));
    ui->radioButton->setChecked(true);

    m_pGroup = new QButtonGroup(this);
    m_pGroup->setExclusive(true);
    m_pGroup->addButton(ui->radioButton, 0);
    m_pGroup->addButton(ui->radioButton_2, 1);

    setupButtons();
    setupCompleter();
    ToolApplyHighlighter(ui->plainTextEdit, HighlighterSql);

    // Устанавливаем placeholder text (временно пустой, заполнится в initializePage)
    ui->plainTextEdit->setPlaceholderText("Введите условие проверки существования записи...");

    // Настройка treeView для работы с checkbox'ами
    ui->treeView->setEditTriggers(QAbstractItemView::CurrentChanged);
    ui->treeView->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->treeView->setSelectionMode(QAbstractItemView::SingleSelection);

    // Подключаем обработчики
    connect(ui->treeView, &QTreeView::activated, this, &ExistsConditionPage::onTreeViewActivated);
    connect(m_pGroup, QOverload<int>::of(&QButtonGroup::buttonClicked), ui->stackedWidget, &QStackedWidget::setCurrentIndex);
    connect(ui->plainTextEdit, &QPlainTextEdit::textChanged, this, &ExistsConditionPage::onCustomConditionChanged);
}

ExistsConditionPage::~ExistsConditionPage()
{
    ui->plainTextEdit->disconnect();
    delete ui;
}

void ExistsConditionPage::setupButtons()
{
    // Создаем горизонтальный layout для кнопок
    QHBoxLayout *buttonLayout = new QHBoxLayout();
    buttonLayout->setContentsMargins(0, 0, 0, 0);
    buttonLayout->setSpacing(5);

    // Создаем кнопку "Шаблоны" с выпадающим меню
    m_btnTemplates = new QPushButton("Шаблоны", this);
    m_btnTemplates->setToolTip("Вставить шаблон условия");
    QMenu *templatesMenu = new QMenu(m_btnTemplates);
    m_btnTemplates->setMenu(templatesMenu);

    // Добавляем шаблоны в меню
    QHash<QString, QString> templates = {
        {"Проверка EXISTS", "basic_exists"},
        {"Проверка COUNT", "count_check"},
        {"Одно поле", "single_field"},
        {"Несколько полей", "multiple_fields"},
        {"Диапазон дат", "date_range"},
        {"Поиск по шаблону", "like_pattern"},
        {"Список значений", "in_list"},
        {"Проверка на NULL", "null_check"}
    };

    for (auto it = templates.begin(); it != templates.end(); ++it)
    {
        QAction *action = templatesMenu->addAction(it.key());
        connect(action, &QAction::triggered, this, [this, templateKey = it.value()]() {
            onTemplateSelected(templateKey);
        });
    }

    // Создаем кнопку "Поля" с выпадающим меню
    m_btnFields = new QPushButton("Поля", this);
    m_btnFields->setToolTip("Вставить поле таблицы");
    QMenu *fieldsMenu = new QMenu(m_btnFields);
    m_btnFields->setMenu(fieldsMenu);

    // Создаем кнопку "Функции" с выпадающим меню
    m_btnFunctions = new QPushButton("Функции", this);
    m_btnFunctions->setToolTip("Вставить SQL функцию");
    QMenu *functionsMenu = new QMenu(m_btnFunctions);
    m_btnFunctions->setMenu(functionsMenu);

    // Добавляем функции в меню
    QStringList functions = {
        "TO_DATE", "TO_CHAR", "NVL", "COALESCE", "DECODE",
        "SUBSTR", "INSTR", "LENGTH", "UPPER", "LOWER",
        "TRIM", "LTRIM", "RTRIM", "REPLACE", "LPAD", "RPAD"
    };

    for (const QString &func : functions)
    {
        QAction *action = functionsMenu->addAction(func);
        connect(action, &QAction::triggered, this, [this, func]() {
            onFunctionSelected(func);
        });
    }

    // Создаем кнопку "Параметры" с выпадающим меню
    m_btnParameters = new QPushButton("Параметры", this);
    m_btnParameters->setToolTip("Вставить параметр функции");
    QMenu *parametersMenu = new QMenu(m_btnParameters);
    m_btnParameters->setMenu(parametersMenu);

    // Заполняем меню параметров
    setupParameterMenu();

    // Создаем кнопку "Проверить"
    m_btnValidate = new QPushButton("Проверить", this);
    m_btnValidate->setToolTip("Проверить корректность условия");
    connect(m_btnValidate, &QPushButton::clicked, this, &ExistsConditionPage::onValidateCondition);

    // Добавляем кнопки в layout
    buttonLayout->addWidget(m_btnTemplates);
    buttonLayout->addWidget(m_btnFields);
    buttonLayout->addWidget(m_btnFunctions);
    buttonLayout->addWidget(m_btnParameters);
    buttonLayout->addWidget(m_btnValidate);
    buttonLayout->addStretch();

    // Добавляем layout с кнопками в вертикальный layout страницы
    ui->verticalLayout_2->insertLayout(0, buttonLayout);

    // Добавляем информационную подсказку
    m_hintLabel = new QLabel(
        "<b>Советы по использованию:</b>"
        "<ul>"
        "<li><b>Выделите текст</b> или <b>установите курсор на поле</b> перед вставкой шаблона/функции</li>"
        "<li>Система автоматически заменит <code>field_name</code> на выделенный текст</li>"
        "<li>Используйте <b>Параметры</b> (p_col1, p_col2...) для ссылок на данные из Word таблицы</li>"
        "<li>Количество параметров соответствует количеству колонок в Word таблице</li>"
        "<li>Пример условия: <code>t_code = p_col1 AND t_status = 1</code></li>"
        "<li>Для проверки связей: <code>EXISTS (SELECT 1 FROM users WHERE id = p_col2)</code></li>"
        "</ul>",
        this
        );
    m_hintLabel->setWordWrap(true);
    m_hintLabel->setStyleSheet("QLabel { color: #444; background: #f0f8ff; padding: 10px; border: 1px solid #b0c4de; border-radius: 4px; margin: 5px; }");
    m_hintLabel->setTextFormat(Qt::RichText);

    ui->verticalLayout_2->insertWidget(1, m_hintLabel);
}

void ExistsConditionPage::setupCompleter()
{
    m_completer = new QCompleter(this);
    m_completer->setCaseSensitivity(Qt::CaseInsensitive);
    m_completer->setCompletionMode(QCompleter::PopupCompletion);
    m_completer->setFilterMode(Qt::MatchContains);

    // Создаем модель с категориями
    m_completerModel = new QStandardItemModel(this);
    m_completer->setModel(m_completerModel);

    /*// Устанавливаем completer для plainTextEdit
    ui->plainTextEdit->setCompleter(m_completer);*/
}

void ExistsConditionPage::setupParameterMenu()
{
    if (!m_btnParameters)
        return;

    QMenu *parametersMenu = m_btnParameters->menu();
    if (!parametersMenu)
        return;

    parametersMenu->clear();

    // Получаем список параметров
    QStringList parameters = getFunctionParameters();

    for (const QString &param : parameters)
    {
        QAction *action = parametersMenu->addAction(param);
        connect(action, &QAction::triggered, this, [this, param]() {
            onParameterSelected(param);
        });
    }
}

QTextTable* ExistsConditionPage::findFirstTable(QTextDocument *document) const
{
    if (!document)
        return nullptr;

    QTextFrame *rootFrame = document->rootFrame();
    if (!rootFrame)
        return nullptr;

    // Рекурсивно ищем таблицу в документе
    return findTableInFrame(rootFrame);
}

int ExistsConditionPage::findTableColumnCount(QTextDocument *document) const
{
    if (!document)
        return 0;

    // Ищем первую таблицу в документе
    QTextTable *table = findFirstTable(document);
    if (table)
    {
        return table->columns();
    }

    return 0;
}

QTextTable* ExistsConditionPage::findTableInFrame(QTextFrame *frame) const
{
    if (!frame)
        return nullptr;

    // Проверяем сам фрейм (может быть таблицей)
    QTextTable *table = qobject_cast<QTextTable*>(frame);
    if (table)
        return table;

    // Ищем в дочерних фреймах
    QTextFrame::iterator it;
    for (it = frame->begin(); !it.atEnd(); ++it)
    {
        QTextFrame *childFrame = it.currentFrame();
        if (childFrame)
        {
            QTextTable *foundTable = findTableInFrame(childFrame);
            if (foundTable)
                return foundTable;
        }
    }

    return nullptr;
}

QStringList ExistsConditionPage::getFunctionParameters() const
{
    QStringList parameters;

    RichTextToInsertWizard *wzrd = qobject_cast<RichTextToInsertWizard*>(wizard());
    if (!wzrd)
    {
        // Fallback: фиксированные параметры
        for (int i = 1; i <= 10; ++i)
        {
            parameters << QString("p_col%1").arg(i);
        }
        return parameters;
    }

    // Получаем документ из мастера
    QTextDocument *document = wzrd->document();
    if (!document)
    {
        // Fallback: фиксированные параметры
        for (int i = 1; i <= 10; ++i)
        {
            parameters << QString("p_col%1").arg(i);
        }
        return parameters;
    }

    // Находим таблицу в документе и определяем количество колонок
    int columnCount = findTableColumnCount(document);

    if (columnCount > 0)
    {
        // Создаем параметры на основе реального количества колонок
        for (int i = 1; i <= columnCount; ++i)
        {
            parameters << QString("p_col%1").arg(i);
        }
    }
    else
    {
        // Если не нашли таблицу, используем разумное ограничение
        for (int i = 1; i <= 10; ++i)
        {
            parameters << QString("p_col%1").arg(i);
        }
    }

    return parameters;
}

QString ExistsConditionPage::getSelectedText() const
{
    QTextCursor cursor = ui->plainTextEdit->textCursor();

    // Если есть выделенный текст - используем его
    if (cursor.hasSelection())
    {
        QString selectedText = cursor.selectedText().trimmed();
        if (!selectedText.isEmpty() && isFieldName(selectedText))
        {
            return selectedText;
        }
    }

    // Если нет выделения, проверяем слово под курсором
    cursor.select(QTextCursor::WordUnderCursor);
    QString wordUnderCursor = cursor.selectedText().trimmed();
    if (!wordUnderCursor.isEmpty() && isFieldName(wordUnderCursor))
    {
        return wordUnderCursor;
    }

    return QString();
}

void ExistsConditionPage::onTreeViewActivated(const QModelIndex &index)
{
    if (!m_pModel)
        return;

    if (m_pModel->isIndex(index))
    {
        Qt::CheckState currentState = static_cast<Qt::CheckState>(
            index.data(Qt::CheckStateRole).toInt());

        Qt::CheckState newState = (currentState == Qt::Checked) ? Qt::Unchecked : Qt::Checked;
        m_pModel->setData(index, newState, Qt::CheckStateRole);
    }
}

void ExistsConditionPage::onSelectionChanged(FmtIndex* selectedIndex)
{
    Q_UNUSED(selectedIndex);
    emit completeChanged();
}

void ExistsConditionPage::onCustomConditionChanged()
{
    updateValidationState();
    emit completeChanged();
}

void ExistsConditionPage::onTextCursorChanged()
{
    if (m_conditionHelper && m_completer)
    {
        QStringList completionList;
        completionList << m_conditionHelper->getFieldNames()
                       << m_conditionHelper->getFunctions()
                       << m_conditionHelper->getSqlKeywords();

        m_completer->setModel(new QStringListModel(completionList, m_completer));
    }
}

void ExistsConditionPage::onValidateCondition()
{
    /*if (!m_conditionHelper || !ui->radioButton_2->isChecked())
        return;

    QString condition = ui->plainTextEdit->toPlainText().trimmed();

    if (condition.isEmpty())
    {
        QMessageBox::warning(this, "Ошибка", "Условие не может быть пустым");
        m_isConditionValid = false;
        m_validationError = "Условие не может быть пустым";
    }
    else
    {
        QString error = m_conditionHelper->validateCondition(condition);

        if (error.isEmpty())
        {
            QMessageBox::information(this, "Проверка условия",
                                     "Условие корректно!\n\n"
                                     "Помните: система не проверяет существование полей в других таблицах.\n"
                                     "Убедитесь, что логика условия соответствует вашей бизнес-задаче.");

            m_isConditionValid = true;
            m_validationError.clear();
        }
        else
        {
            QMessageBox::warning(this, "Ошибка в условии", error);
            m_isConditionValid = false;
            m_validationError = error;
        }
    }

    updateValidationState();
    emit completeChanged();*/
    if (!m_conditionHelper || !ui->radioButton_2->isChecked())
        return;

    QString condition = ui->plainTextEdit->toPlainText().trimmed();

    if (condition.isEmpty())
    {
        QMessageBox::warning(this, "Ошибка", "Условие не может быть пустым");
        m_isConditionValid = false;
        m_validationError = "Условие не может быть пустым";
    }
    else
    {
        QString error = m_conditionHelper->validateCondition(condition);

        // Дополнительная проверка: валидация параметров
        QStringList parameters = getFunctionParameters();
        QRegularExpression paramRegex("\\b(p_col\\d+)\\b");
        QRegularExpressionMatchIterator it = paramRegex.globalMatch(condition);

        QSet<QString> usedParams;
        while (it.hasNext())
        {
            QRegularExpressionMatch match = it.next();
            usedParams.insert(match.captured(1));
        }

        // Проверяем, все ли использованные параметры существуют
        QStringList invalidParams;
        for (const QString &param : usedParams)
        {
            if (!parameters.contains(param))
            {
                invalidParams << param;
            }
        }

        if (!invalidParams.isEmpty())
        {
            error = QString("Используются несуществующие параметры: %1\n"
                            "Доступные параметры: %2")
                        .arg(invalidParams.join(", "))
                        .arg(parameters.join(", "));
        }

        if (error.isEmpty())
        {
            QMessageBox::information(this, "Проверка условия",
                                     "Условие корректно!\n\n"
                                     "Помните: система не проверяет существование полей в других таблицах.\n"
                                     "Убедитесь, что логика условия соответствует вашей бизнес-задаче.");

            m_isConditionValid = true;
            m_validationError.clear();
        }
        else
        {
            QMessageBox::warning(this, "Ошибка в условии", error);
            m_isConditionValid = false;
            m_validationError = error;
        }
    }

    updateValidationState();
    emit completeChanged();
}

void ExistsConditionPage::onTemplateSelected(const QString &templateName)
{
    if (!m_conditionHelper)
        return;

    QString templateText = m_conditionHelper->generateTemplate(templateName);
    if (!templateText.isEmpty())
    {
        // Получаем текст вокруг курсора (выделенный или под курсором)
        QString contextText = getSelectedText();

        // Если есть контекстный текст, используем его для замены
        if (!contextText.isEmpty())
        {
            templateText = templateText.replace("field_name", contextText, Qt::CaseInsensitive);
            templateText = templateText.replace("field1", contextText, Qt::CaseInsensitive);
            templateText = templateText.replace("field2", contextText, Qt::CaseInsensitive);
        }

        // Вставляем шаблон
        QTextCursor cursor = ui->plainTextEdit->textCursor();

        // Если есть выделенный текст или слово под курсором - заменяем его
        if (cursor.hasSelection() || !contextText.isEmpty())
        {
            // Если было выделение, заменяем выделение
            // Если не было выделения, но есть слово под курсором, заменяем его
            if (!cursor.hasSelection() && !contextText.isEmpty())
            {
                cursor.select(QTextCursor::WordUnderCursor);
            }
            cursor.insertText(templateText);
        }
        else
        {
            // Иначе просто вставляем в позицию курсора
            ui->plainTextEdit->insertPlainText(templateText);
        }
    }
}

void ExistsConditionPage::onFieldSelected(const QString &fieldName)
{
    QTextCursor cursor = ui->plainTextEdit->textCursor();

    // Если есть выделенный текст или слово под курсором - заменяем его
    QString contextText = getSelectedText();
    if (!contextText.isEmpty())
    {
        // Если было выделение, заменяем выделение
        // Если не было выделения, но есть слово под курсором, заменяем его
        if (!cursor.hasSelection() && !contextText.isEmpty())
        {
            cursor.select(QTextCursor::WordUnderCursor);
        }
        cursor.insertText(fieldName);
    }
    else
    {
        // Иначе просто вставляем в позицию курсора
        ui->plainTextEdit->insertPlainText(fieldName);
    }
}

void ExistsConditionPage::onFunctionSelected(const QString &functionName)
{
    QHash<QString, QString> functionTemplates = {
        {"TO_DATE", "TO_DATE('01.01.2024', 'DD.MM.YYYY')"},
        {"TO_CHAR", "TO_CHAR(SYSDATE, 'DD.MM.YYYY')"},
        {"NVL", "NVL(field_name, 'default_value')"},
        {"COALESCE", "COALESCE(field_name, field_name2, 'default_value')"},
        {"SUBSTR", "SUBSTR(field_name, 1, 10)"},
        {"UPPER", "UPPER(field_name)"},
        {"LOWER", "LOWER(field_name)"}
    };

    QString templateText = functionTemplates.value(functionName, functionName + "(field_name)");

    // Получаем текст вокруг курсора (выделенный или под курсором)
    QString contextText = getSelectedText();

    // Если есть контекстный текст, используем его для замены
    if (!contextText.isEmpty())
    {
        templateText = templateText.replace("field_name", contextText, Qt::CaseInsensitive);
        templateText = templateText.replace("field_name2", contextText + "2", Qt::CaseInsensitive);
    }

    // Вставляем функцию
    QTextCursor cursor = ui->plainTextEdit->textCursor();

    // Если есть выделенный текст или слово под курсором - заменяем его
    if (cursor.hasSelection() || !contextText.isEmpty())
    {
        // Если было выделение, заменяем выделение
        // Если не было выделения, но есть слово под курсором, заменяем его
        if (!cursor.hasSelection() && !contextText.isEmpty())
        {
            cursor.select(QTextCursor::WordUnderCursor);
        }
        cursor.insertText(templateText);
    }
    else
    {
        // Иначе просто вставляем в позицию курсора
        ui->plainTextEdit->insertPlainText(templateText);
    }
}

void ExistsConditionPage::onParameterSelected(const QString &paramName)
{
    QTextCursor cursor = ui->plainTextEdit->textCursor();

    // Если есть выделенный текст или слово под курсором - заменяем его
    QString contextText = getSelectedText();
    if (!contextText.isEmpty())
    {
        // Если было выделение, заменяем выделение
        // Если не было выделения, но есть слово под курсором, заменяем его
        if (!cursor.hasSelection() && !contextText.isEmpty())
        {
            cursor.select(QTextCursor::WordUnderCursor);
        }
        cursor.insertText(paramName);
    }
    else
    {
        // Иначе просто вставляем в позицию курсора
        ui->plainTextEdit->insertPlainText(paramName);
    }
}

void ExistsConditionPage::insertFieldPlaceholder(const QString &fieldName)
{
    QTextCursor cursor = ui->plainTextEdit->textCursor();

    // Если есть выделенный текст - заменяем его на имя поля
    if (cursor.hasSelection())
    {
        cursor.insertText(fieldName);
    }
    else
    {
        ui->plainTextEdit->insertPlainText(fieldName);
    }
}

void ExistsConditionPage::insertFunctionTemplate(const QString &functionName)
{
    onFunctionSelected(functionName);
}

void ExistsConditionPage::insertParameterPlaceholder(const QString &paramName)
{
    QTextCursor cursor = ui->plainTextEdit->textCursor();

    // Если есть выделенный текст - заменяем его на параметр
    if (cursor.hasSelection())
    {
        cursor.insertText(paramName);
    }
    else
    {
        ui->plainTextEdit->insertPlainText(paramName);
    }
}

QString ExistsConditionPage::formatFieldPlaceholder(const QString &fieldName) const
{
    return fieldName;
}

QString ExistsConditionPage::extractFieldNameFromPlaceholder(const QString &placeholder) const
{
    QRegularExpression regex("\\{([^}]+)\\}");
    QRegularExpressionMatch match = regex.match(placeholder);
    if (match.hasMatch())
        return match.captured(1);
    return placeholder;
}

QSet<QString> ExistsConditionPage::getUsedFieldsInCondition() const
{
    QSet<QString> usedFields;
    QString condition = ui->plainTextEdit->toPlainText();

    QRegularExpression placeholderRegex("\\{([^}]+)\\}");
    QRegularExpressionMatchIterator it = placeholderRegex.globalMatch(condition);

    while (it.hasNext())
    {
        QRegularExpressionMatch match = it.next();
        usedFields.insert(match.captured(1));
    }

    return usedFields;
}

void ExistsConditionPage::updateValidationState()
{
    QString condition = ui->plainTextEdit->toPlainText().trimmed();

    if (ui->radioButton_2->isChecked())
    {
        if (condition.isEmpty())
        {
            m_isConditionValid = false;
        }
        else
        {
            m_isConditionValid = true;
            m_validationError.clear();
        }
    }
    else
    {
        m_isConditionValid = (m_pModel && m_pModel->getSelectedIndex() != nullptr);
        m_validationError = m_isConditionValid ? "" : "Выберите индекс для проверки";
    }
}

void ExistsConditionPage::initializePage()
{
    RichTextToInsertWizard *wzrd = qobject_cast<RichTextToInsertWizard*>(wizard());

    if (m_pModel)
    {
        delete m_pModel;
        m_pModel = nullptr;
    }

    if (m_conditionHelper)
    {
        delete m_conditionHelper;
        m_conditionHelper = nullptr;
    }

    if (wzrd && wzrd->table())
    {
        m_pModel = new FmtIndexTreeModel(wzrd->table(), this);
        m_conditionHelper = new CustomConditionHelper(wzrd->table(), this);

        connect(m_pModel, &FmtIndexTreeModel::selectionChanged,
                this, &ExistsConditionPage::onSelectionChanged);

        // Заполняем меню полей
        if (m_btnFields && m_conditionHelper)
        {
            QMenu *fieldsMenu = m_btnFields->menu();
            if (fieldsMenu)
            {
                fieldsMenu->clear();
                QStringList fields = m_conditionHelper->getFieldNames();

                for (const QString &field : fields)
                {
                    QAction *action = fieldsMenu->addAction(field);
                    connect(action, &QAction::triggered, this, [this, field]() {
                        onFieldSelected(field);
                    });
                }
            }
        }

        // Обновляем меню параметров на основе реального документа
        setupParameterMenu();

        // Устанавливаем placeholder text с примером
        updatePlaceholderText();

        // Обновляем подсказку с информацией о количестве параметров
        updateHintLabel();
    }

    ui->treeView->setModel(m_pModel);
    if (m_pModel)
    {
        ui->treeView->expandAll();
    }

    updateValidationState();
}

void ExistsConditionPage::updateHintLabel()
{
    if (!m_hintLabel)
        return;

    QStringList parameters = getFunctionParameters();
    int paramCount = parameters.size();

    QString hintText = QString(
                           "<b>Советы по использованию:</b>"
                           "<ul>"
                           "<li><b>Выделите текст</b> или <b>установите курсор на поле</b> перед вставкой шаблона/функции</li>"
                           "<li>Система автоматически заменит <code>field_name</code> на выделенный текст</li>"
                           "<li>Используйте <b>Параметры</b> (p_col1, p_col2...) для ссылок на данные из Word таблицы</li>"
                           "<li><b>Доступно параметров: %1</b> (по количеству колонок в Word таблице)</li>"
                           "<li>Пример условия: <code>t_code = p_col1 AND t_status = 1</code></li>"
                           "<li>Для проверки связей: <code>EXISTS (SELECT 1 FROM users WHERE id = p_col2)</code></li>"
                           "</ul>"
                           "<b>Доступные параметры:</b> %2")
                           .arg(paramCount)
                           .arg(parameters.join(", "));

    m_hintLabel->setText(hintText);
}

bool ExistsConditionPage::validatePage()
{
    updateValidationState();

    if (ui->radioButton_2->isChecked() && !m_isConditionValid)
    {
        if (ui->plainTextEdit->toPlainText().trimmed().isEmpty())
        {
            QMessageBox::warning(this, "Ошибка", "Условие не может быть пустым");
        }
        else
        {
            QMessageBox::warning(this, "Ошибка",
                                 m_validationError.isEmpty() ? "Условие содержит ошибки" : m_validationError);
        }
        return false;
    }

    if (ui->radioButton->isChecked() && !m_pModel->getSelectedIndex())
    {
        QMessageBox::warning(this, "Ошибка", "Выберите индекс для проверки существования записи");
        return false;
    }

    return true;
}

bool ExistsConditionPage::isComplete() const
{
    if (ui->radioButton->isChecked())
        return m_pModel && m_pModel->getSelectedIndex() != nullptr;
    else
        return m_isConditionValid;
}

FmtIndex* ExistsConditionPage::getSelectedIndex() const
{
    return m_pModel ? m_pModel->getSelectedIndex() : nullptr;
}

QString ExistsConditionPage::getCustomCondition() const
{
    return ui->plainTextEdit->toPlainText().trimmed();
}

bool ExistsConditionPage::useCustomCondition() const
{
    return ui->radioButton_2->isChecked();
}

bool ExistsConditionPage::isConditionValid() const
{
    return m_isConditionValid;
}

QString ExistsConditionPage::getValidationError() const
{
    return m_validationError;
}

void ExistsConditionPage::updatePlaceholderText()
{
    if (!m_conditionHelper || !m_pModel)
        return;

    QStringList fields = m_conditionHelper->getFieldNames();
    if (fields.isEmpty())
        return;

    // Ищем первичный ключ для примера
    QString primaryKeyField;
    if (m_pModel)
    {
        // Ищем первый первичный ключ в модели
        for (int i = 0; i < m_pModel->rowCount(); ++i)
        {
            QModelIndex index = m_pModel->index(i, 0);
            FmtIndex *fmtIndex = m_pModel->getIndex(index);
            if (fmtIndex && fmtIndex->isPrimary())
            {
                // Берем первое поле из первичного ключа
                if (fmtIndex->segmentsCount() > 0)
                {
                    FmtSegment *segment = fmtIndex->segment(0);
                    if (segment && segment->field())
                    {
                        primaryKeyField = segment->field()->name();
                        break;
                    }
                }
            }
        }
    }

    // Если не нашли первичный ключ, берем первое поле
    if (primaryKeyField.isEmpty() && !fields.isEmpty())
    {
        primaryKeyField = fields.first();
    }

    // Создаем пример условия
    QString example;
    if (!primaryKeyField.isEmpty())
    {
        example = QString("%1 = p_col1").arg(primaryKeyField);
    }
    else
    {
        example = "field_name = p_col1";
    }

    /*ui->plainTextEdit->setPlaceholderText(
        QString("Пример: %1\nМожно использовать поля таблицы, параметры (p_col1, p_col2...) и SQL функции")
            .arg(example)
        );*/
}

bool ExistsConditionPage::isFieldName(const QString &text) const
{
    if (text.isEmpty() || !m_conditionHelper)
        return false;

    // Убираем пробелы и приводим к верхнему регистру для проверки
    QString cleanedText = text.toUpper().remove(' ');

    QStringList fieldNames = m_conditionHelper->getFieldNames();
    for (const QString &fieldName : fieldNames)
    {
        if (fieldName.toUpper().remove(' ') == cleanedText)
        {
            return true;
        }
    }

    return false;
}
