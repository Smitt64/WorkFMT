#ifndef EXISTSCONDITIONPAGE_H
#define EXISTSCONDITIONPAGE_H

#include <QWizardPage>
#include <QAbstractItemModel>
#include <QSet>
#include <QStringListModel>

class FmtTable;
class FmtIndex;
class FmtSegment;
class FmtField;
class Ui_ExistsConditionPage;
class QButtonGroup;
class QCompleter;
class QPushButton;
class QLabel;
class RichTextToInsertWizard;

// Внутренний класс для дерева индексов
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

    QVariant data(int column) const;

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

class FmtIndexTreeModel : public QAbstractItemModel
{
    Q_OBJECT

public:
    explicit FmtIndexTreeModel(FmtTable *table, QObject *parent = nullptr);
    ~FmtIndexTreeModel();

    // QAbstractItemModel interface
    QModelIndex index(int row, int column, const QModelIndex &parent = QModelIndex()) const override;
    QModelIndex parent(const QModelIndex &child) const override;
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    int columnCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    bool setData(const QModelIndex &index, const QVariant &value, int role) override;
    Qt::ItemFlags flags(const QModelIndex &index) const override;

    // Custom methods
    FmtIndex* getIndex(const QModelIndex &index) const;
    bool isIndex(const QModelIndex &index) const;
    bool isSegment(const QModelIndex &index) const;

    // Selection methods
    FmtIndex* getSelectedIndex() const;
    QModelIndex getSelectedIndexModelIndex() const;
    void setSelectedIndex(const QModelIndex &index);
    void clearSelection();

signals:
    void selectionChanged(FmtIndex* selectedIndex);

private:
    void setupModelData();
    QModelIndex findIndexForIndex(FmtIndex *index) const;
    FmtIndexTreeItem *getItem(const QModelIndex &index) const;

private:
    FmtIndexTreeItem *m_rootItem = nullptr;
    FmtTable *m_table = nullptr;
    FmtIndex *m_selectedIndex = nullptr;
};

// -------------------------------------------------------------------------------------

class CustomConditionHelper : public QObject
{
    Q_OBJECT
public:
    explicit CustomConditionHelper(FmtTable *table, QObject *parent = nullptr);

    QStringList getFieldNames() const;
    QStringList getTableNames() const;
    QStringList getSqlKeywords() const;
    QStringList getFunctions() const;
    QString generateTemplate(const QString &templateName) const;
    QString validateCondition(const QString &condition) const;

private:
    FmtTable *m_table = nullptr;
};

// -------------------------------------------------------------------------------------

class QTextDocument;
class QTextTable;
class QTextFrame;
class QStandardItemModel;
class ExistsConditionPage : public QWizardPage
{
    Q_OBJECT

public:
    explicit ExistsConditionPage(QWidget *parent = nullptr);
    ~ExistsConditionPage();

    void initializePage() override;
    bool validatePage() override;
    bool isComplete() const override;
    FmtIndex* getSelectedIndex() const;
    QString getCustomCondition() const;
    bool useCustomCondition() const;
    bool isConditionValid() const;
    QString getValidationError() const;

    // Новые методы для работы с условиями
    void insertFieldPlaceholder(const QString &fieldName);
    void insertFunctionTemplate(const QString &functionName);
    QStringList getFunctionParameters() const;
    int findTableColumnCount(QTextDocument *document) const;
    void insertParameterPlaceholder(const QString &paramName);

private slots:
    void onTreeViewActivated(const QModelIndex &index);
    void onSelectionChanged(FmtIndex* selectedIndex);
    void onCustomConditionChanged();
    void onTextCursorChanged();
    void onValidateCondition();
    void onTemplateSelected(const QString &templateName);
    void onFieldSelected(const QString &fieldName);
    void onFunctionSelected(const QString &functionName);
    void onParameterSelected(const QString &paramName);

private:
    void setupButtons();
    void setupCompleter();
    void updateValidationState();
    QString formatFieldPlaceholder(const QString &fieldName) const;
    QString extractFieldNameFromPlaceholder(const QString &placeholder) const;
    QSet<QString> getUsedFieldsInCondition() const;
    void setupParameterMenu();
    QString getSelectedText() const;
    void updatePlaceholderText();
    bool isFieldName(const QString &text) const;

    QTextTable* findFirstTable(QTextDocument *document) const;
    QTextTable* findTableInFrame(QTextFrame *frame) const;
    void updateHintLabel();

private:
    Ui_ExistsConditionPage *ui;
    FmtIndexTreeModel *m_pModel;
    QButtonGroup *m_pGroup;
    CustomConditionHelper *m_conditionHelper;
    QCompleter *m_completer;
    bool m_isConditionValid;
    QString m_validationError;

    // Кнопки вместо toolbar
    QPushButton *m_btnTemplates;
    QPushButton *m_btnFields;
    QPushButton *m_btnFunctions;
    QPushButton *m_btnParameters;
    QPushButton *m_btnValidate;
    QLabel *m_hintLabel;

    QStandardItemModel *m_completerModel;
};

#endif // EXISTSCONDITIONPAGE_H
