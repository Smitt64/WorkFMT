#include "existsconditionpage.h"
#include "src/wizards/RichTextToInsertWizard/richtexttoinsertwizard.h"
#include "ui_existsconditionpage.h"
#include "fmttable.h"
#include "fmtindex.h"
#include "fmtsegment.h"
#include "codeeditor/codehighlighter.h"
#include <QIcon>
#include <QButtonGroup>
#include <QCompleter>
#include <functional>

class FmtIndexTreeItem
{
public:
    // Конструктор для корневого элемента
    explicit FmtIndexTreeItem()
        : m_index(nullptr), m_segment(nullptr), m_parent(nullptr) {}

    // Конструктор для элемента индекса
    explicit FmtIndexTreeItem(FmtIndex *index, FmtIndexTreeItem *parent = nullptr)
        : m_index(index), m_segment(nullptr), m_parent(parent) {}

    // Конструктор для элемента сегмента
    explicit FmtIndexTreeItem(FmtSegment *segment, FmtIndexTreeItem *parent = nullptr)
        : m_index(nullptr), m_segment(segment), m_parent(parent) {}

    ~FmtIndexTreeItem()
    {
        qDeleteAll(m_children);
    }

    void appendChild(FmtIndexTreeItem *child)
    {
        m_children.append(child);
    }

    FmtIndexTreeItem *child(int row)
    {
        if (row < 0 || row >= m_children.size())
            return nullptr;
        return m_children.at(row);
    }

    int childCount() const
    {
        return m_children.size();
    }

    int columnCount() const
    {
        return 1;
    }

    QVariant data(int column) const
    {
        if (m_index)
        {
            if (column == 0)
                return m_index->name();
        }
        else if (m_segment)
        {
            if (column == 0)
                return m_segment->field() ? m_segment->field()->undecorateName() : QString();
        }
        return QVariant();
    }

    int row() const
    {
        if (m_parent)
            return m_parent->m_children.indexOf(const_cast<FmtIndexTreeItem*>(this));
        return 0;
    }

    FmtIndexTreeItem *parent()
    {
        return m_parent;
    }

    FmtIndex *index() const { return m_index; }
    FmtSegment *segment() const { return m_segment; }
    bool isIndex() const { return m_index != nullptr; }
    bool isSegment() const { return m_segment != nullptr; }
    bool isRoot() const { return m_index == nullptr && m_segment == nullptr; }

private:
    FmtIndex *m_index = nullptr;
    FmtSegment *m_segment = nullptr;
    FmtIndexTreeItem *m_parent = nullptr;
    QList<FmtIndexTreeItem*> m_children;
};

// -------------------------------------------------------------------------------------

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

// -------------------------------------------------------------------------------------

ExistsConditionPage::ExistsConditionPage(QWidget *parent)
    : QWizardPage(parent)
    , ui(new Ui::ExistsConditionPage),
    m_pModel(nullptr),
    m_pGroup(nullptr)
{
    ui->setupUi(this);
    ui->radioButton->setChecked(true);

    m_pGroup = new QButtonGroup(this);
    m_pGroup->setExclusive(true);
    m_pGroup->addButton(ui->radioButton, 0);
    m_pGroup->addButton(ui->radioButton_2, 1);

    QStringList wordList;
    wordList << "function" << "class" << "void" << "int" << "double" << "string"
             << "return" << "if" << "else" << "for" << "while" << "switch";

    QCompleter *completer = new QCompleter(wordList, this);
    completer->setCaseSensitivity(Qt::CaseInsensitive);
    completer->setCompletionMode(QCompleter::PopupCompletion);

    ToolApplyHighlighter(ui->plainTextEdit, HighlighterSql);

    // Настройка treeView для работы с checkbox'ами
    ui->treeView->setEditTriggers(QAbstractItemView::CurrentChanged);
    ui->treeView->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->treeView->setSelectionMode(QAbstractItemView::SingleSelection);

    // Подключаем обработчики
    connect(ui->treeView, &QTreeView::activated, this, &ExistsConditionPage::onTreeViewActivated);
    connect(m_pGroup, &QButtonGroup::idClicked, ui->stackedWidget, &QStackedWidget::setCurrentIndex);
    connect(ui->plainTextEdit, &QPlainTextEdit::textChanged, this, &ExistsConditionPage::onCustomConditionChanged);
}

ExistsConditionPage::~ExistsConditionPage()
{
    delete ui;
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
    // Можно добавить дополнительную логику при изменении выбора
    Q_UNUSED(selectedIndex);
}

void ExistsConditionPage::onCustomConditionChanged()
{
    // Обработка изменения пользовательского условия
}

void ExistsConditionPage::initializePage()
{
    RichTextToInsertWizard *wzrd = qobject_cast<RichTextToInsertWizard*>(wizard());

    if (m_pModel)
    {
        delete m_pModel;
        m_pModel = nullptr;
    }

    if (wzrd && wzrd->table())
    {
        m_pModel = new FmtIndexTreeModel(wzrd->table(), this);
        connect(m_pModel, &FmtIndexTreeModel::selectionChanged,
                this, &ExistsConditionPage::onSelectionChanged);
    }

    ui->treeView->setModel(m_pModel);
    if (m_pModel)
    {
        ui->treeView->expandAll();
    }
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
