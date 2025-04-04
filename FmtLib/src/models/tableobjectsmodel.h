#ifndef TABLEOBJECTSMODEL_H
#define TABLEOBJECTSMODEL_H

#include <QAbstractItemModel>

enum TreeItemType
{
    RootItem,
    CategoryItem,
    ObjectItem,
    IndexItem,
    TriggerItem,
    ConstraintItem,
    SequenceItem
};

struct TreeNode
{
    QString name;
    QString sql;
    TreeItemType type;
    Qt::CheckState checkState;
    TreeNode* parent;
    QList<TreeNode*> children;

    ~TreeNode()
    {
        qDeleteAll(children);
    }

    QString objectTypeString() const
    {
        switch(type)
        {
        case RootItem: return "Table";
        case CategoryItem: return "Category";
        case IndexItem: return "Index";
        case TriggerItem: return "Trigger";
        case ConstraintItem: return "Constraint";
        case SequenceItem: return "Sequence";
        default: return "Unknown";
        }
    }
};

class ConnectionInfo;
class TableObjectsModel : public QAbstractItemModel
{
    Q_OBJECT
public:
    enum class DbType
    {
        PostgreSQL,
        Oracle
    };

    TableObjectsModel(ConnectionInfo *connection, const QString &_tableName, QObject *parent = nullptr);
    virtual ~TableObjectsModel();

    QVariant data(const QModelIndex &index, int role) const override;
    Qt::ItemFlags flags(const QModelIndex &index) const override;
    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;
    QModelIndex index(int row, int column, const QModelIndex &parent = QModelIndex()) const override;
    QModelIndex parent(const QModelIndex &index) const override;
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    int columnCount(const QModelIndex &parent = QModelIndex()) const override;
    bool setData(const QModelIndex &index, const QVariant &value, int role = Qt::EditRole) override;

    QString getSqlForIndex(const QModelIndex &index, bool simplified = false) const;
    void refreshModel();

    QString getObjectType(const QModelIndex &index) const;

private:
    void setupModelData();
    TreeNode* addCategory(TreeNode *parent, const QString &categoryName);
    void addObjectsToCategory(TreeNode *category, const QStringList &objects, const QString &prefix = "");

    // Методы для работы с PostgreSQL
    QString getTableDefinitionPostgres() const;
    QString getIndexesPostgres() const;
    QString getTriggersPostgres() const;
    QString getConstraintsPostgres() const;
    QString getSequencesPostgres() const;
    QString getIndexDefinitionPostgres(const QString &indexName) const;
    QString getTriggerDefinitionPostgres(const QString &triggerName) const;
    QString getConstraintDefinitionPostgres(const QString &constraintName) const;
    QString getSequenceDefinitionPostgres(const QString &sequenceName) const;

    // Методы для работы с Oracle
    QString getTableDefinitionOracle() const;
    QString getIndexesOracle() const;
    QString getTriggersOracle() const;
    QString getConstraintsOracle() const;
    QString getSequencesOracle() const;
    QString getIndexDefinitionOracle(const QString &indexName) const;
    QString getTriggerDefinitionOracle(const QString &triggerName) const;
    QString getConstraintDefinitionOracle(const QString &constraintName) const;
    QString getSequenceDefinitionOracle(const QString &sequenceName) const;

    QString simplifyIndexDDL(const QString &originalDDL) const;
    QString simplifySequenceDDL(const QString &originalDDL) const;
    QString simplifyConstraintDDL(const QString &originalDDL) const;
    QString simplifyTriggerDDL(const QString &originalDDL) const;

    ConnectionInfo *m_pConnection;
    DbType dbType;
    QString tableName;
    TreeNode *rootItem;
};

#endif // TABLEOBJECTSMODEL_H
