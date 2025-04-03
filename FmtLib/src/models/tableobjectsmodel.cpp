#include "tableobjectsmodel.h"
#include "connectioninfo.h"
#include "fmtcore.h"
#include <QSqlQuery>
#include <QSqlError>

TableObjectsModel::TableObjectsModel(ConnectionInfo *connection, const QString &_tableName, QObject *parent) :
    QAbstractItemModel(parent),
    m_pConnection(connection),
    tableName(_tableName)
{
    dbType = DbType::Oracle;

    if (m_pConnection->type() == ConnectionInfo::CON_POSTGRESQL)
        dbType = DbType::PostgreSQL;

    rootItem = new TreeNode{tableName, "", RootItem, Qt::Checked, nullptr};
    setupModelData();
}

TableObjectsModel::~TableObjectsModel()
{
    delete rootItem;
}

QVariant TableObjectsModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();

    TreeNode *item = static_cast<TreeNode*>(index.internalPointer());

    if (role == Qt::DisplayRole)
        return item->name;

    else if (role == Qt::CheckStateRole && item->type == ObjectItem)
        return item->checkState;
    else if (role == Qt::CheckStateRole && item->type == CategoryItem)
    {
        if (item->children.isEmpty())
            return Qt::Unchecked;

        int checkedCount = 0;
        for (const TreeNode* child : qAsConst(item->children))
            if (child->checkState == Qt::Checked) checkedCount++;

        if (checkedCount == 0) return Qt::Unchecked;
        if (checkedCount == item->children.size()) return Qt::Checked;

        return Qt::PartiallyChecked;
    }

    return QVariant();
}

Qt::ItemFlags TableObjectsModel::flags(const QModelIndex &index) const
{
    if (!index.isValid())
        return Qt::NoItemFlags;

    TreeNode *item = static_cast<TreeNode*>(index.internalPointer());
    Qt::ItemFlags flags = QAbstractItemModel::flags(index);

    if (item->type == ObjectItem || item->type == CategoryItem)
        flags |= Qt::ItemIsUserCheckable;

    return flags;
}

QVariant TableObjectsModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (orientation == Qt::Horizontal && role == Qt::DisplayRole && section == 0)
        return "Объекты таблицы";

    return QVariant();
}

QModelIndex TableObjectsModel::index(int row, int column, const QModelIndex &parent) const
{
    if (!hasIndex(row, column, parent))
        return QModelIndex();

    TreeNode *parentItem = parent.isValid() ? static_cast<TreeNode*>(parent.internalPointer()) : rootItem;

    if (row < parentItem->children.size())
        return createIndex(row, column, parentItem->children.at(row));

    return QModelIndex();
}

QModelIndex TableObjectsModel::parent(const QModelIndex &index) const
{
    if (!index.isValid())
        return QModelIndex();

    TreeNode *childItem = static_cast<TreeNode*>(index.internalPointer());
    TreeNode *parentItem = childItem->parent;

    if (parentItem == rootItem || parentItem == nullptr)
        return QModelIndex();

    return createIndex(parentItem->parent->children.indexOf(parentItem), 0, parentItem);
}

int TableObjectsModel::rowCount(const QModelIndex &parent) const
{
    if (parent.column() > 0)
        return 0;

    TreeNode *parentItem = parent.isValid() ? static_cast<TreeNode*>(parent.internalPointer()) : rootItem;
    return parentItem->children.size();
}

int TableObjectsModel::columnCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return 1;
}

bool TableObjectsModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if (role != Qt::CheckStateRole)
        return false;

    TreeNode *item = static_cast<TreeNode*>(index.internalPointer());
    Qt::CheckState state = static_cast<Qt::CheckState>(value.toInt());

    if (item->type == ObjectItem)
    {
        item->checkState = state;
        emit dataChanged(index, index, {Qt::CheckStateRole});

        // Обновляем родительские категории
        QModelIndex parentIndex = index.parent();
        while (parentIndex.isValid())
        {
            emit dataChanged(parentIndex, parentIndex, {Qt::CheckStateRole});
            parentIndex = parentIndex.parent();
        }
        return true;
    }
    else if (item->type == CategoryItem)
    {
        for (int i = 0; i < item->children.size(); ++i)
        {
            item->children[i]->checkState = state;
            QModelIndex childIndex = this->index(i, 0, index);
            emit dataChanged(childIndex, childIndex, {Qt::CheckStateRole});
        }

        return true;
    }

    return false;
}

QString TableObjectsModel::getSqlForIndex(const QModelIndex &index) const
{
    if (!index.isValid())
        return rootItem->sql;

    TreeNode *item = static_cast<TreeNode*>(index.internalPointer());
    return item->sql;
}

void TableObjectsModel::refreshModel()
{
    setupModelData();
}

void TableObjectsModel::setupModelData()
{
    beginResetModel();

    delete rootItem;
    rootItem = new TreeNode{tableName, "", RootItem, Qt::Checked, nullptr};

    // Получаем SQL для создания таблицы
    rootItem->sql = (dbType == DbType::PostgreSQL) ?
        getTableDefinitionPostgres() : getTableDefinitionOracle();

    // Добавляем категории
    TreeNode* indexes = addCategory(rootItem, "Индексы");
    TreeNode* constraints = addCategory(rootItem, "Ограничения");
    TreeNode* sequences = addCategory(rootItem, "Последовательности");
    TreeNode* triggers = addCategory(rootItem, "Триггеры");

    // Заполняем категории объектами
    QStringList indexList = ((dbType == DbType::PostgreSQL) ?
        getIndexesPostgres() : getIndexesOracle()).split(";", Qt::SkipEmptyParts);
    addObjectsToCategory(indexes, indexList);

    QStringList triggerList = ((dbType == DbType::PostgreSQL) ?
        getTriggersPostgres() : getTriggersOracle()).split(";", Qt::SkipEmptyParts);
    addObjectsToCategory(triggers, triggerList);

    QStringList constraintList = ((dbType == DbType::PostgreSQL) ?
        getConstraintsPostgres() : getConstraintsOracle()).split(";", Qt::SkipEmptyParts);
    addObjectsToCategory(constraints, constraintList);

    QStringList sequenceList = ((dbType == DbType::PostgreSQL) ?
        getSequencesPostgres() : getSequencesOracle()).split(";", Qt::SkipEmptyParts);
    addObjectsToCategory(sequences, sequenceList);

    // Удаляем пустые категории
    QList<TreeNode*> categoriesToRemove;
    for (TreeNode* category : qAsConst(rootItem->children))
    {
        if (category->children.isEmpty())
            categoriesToRemove.append(category);
    }

    for (TreeNode* category : categoriesToRemove)
    {
        rootItem->children.removeOne(category);
        delete category;
    }

    for (TreeNode* category : qAsConst(rootItem->children))
    {
        if (!category->children.isEmpty())
        {
            bool allChecked = true;
            bool anyChecked = false;

            for (const TreeNode* child : qAsConst(category->children))
            {
                if (child->checkState == Qt::Checked)
                    anyChecked = true;
                else
                    allChecked = false;
            }

            if (allChecked)
                category->checkState = Qt::Checked;
            else if (anyChecked)
                category->checkState = Qt::PartiallyChecked;
            else
                category->checkState = Qt::Unchecked;
        }
    }

    endResetModel();
}

TreeNode* TableObjectsModel::addCategory(TreeNode *parent, const QString &categoryName)
{
    TreeNode* category = new TreeNode{categoryName, "", CategoryItem, Qt::Unchecked, parent};
    parent->children.append(category);

    return category;
}

void TableObjectsModel::addObjectsToCategory(TreeNode *category, const QStringList &objects, const QString &prefix)
{
    for (const QString &objectName : objects)
    {
        QString sql;
        if (dbType == DbType::PostgreSQL)
        {
            if (category->name == "Индексы")
                sql = getIndexDefinitionPostgres(objectName);

            else if (category->name == "Ограничения")
                sql = getConstraintDefinitionPostgres(objectName);
            else if (category->name == "Последовательности")
                sql = getSequenceDefinitionPostgres(objectName);
            else if (category->name == "Триггеры")
                sql = getTriggerDefinitionPostgres(objectName);
        }
        else
        {
            if (category->name == "Индексы")
                sql = getIndexDefinitionOracle(objectName);

            else if (category->name == "Ограничения")
                sql = getConstraintDefinitionOracle(objectName);
            else if (category->name == "Последовательности")
                sql = getSequenceDefinitionOracle(objectName);
            else if (category->name == "Триггеры")
                sql = getTriggerDefinitionOracle(objectName);
        }

        TreeNode* object = new TreeNode
        {
            prefix + objectName,
            sql,
            ObjectItem,
            Qt::Checked,
            category
        };

        category->children.append(object);
    }
}

QString TableObjectsModel::getTableDefinitionPostgres() const
{
    QSqlQuery query(m_pConnection->db());
    query.prepare("/*@ DisConv */SELECT pg_get_tabledef(?)");
    query.addBindValue(tableName);

    if (!ExecuteQuery(&query) && query.next())
        return query.value(0).toString();

    return QString("Не удалось получить определение таблицы %1").arg(tableName);
}

QString TableObjectsModel::getIndexesPostgres() const
{
    QSqlQuery query(m_pConnection->db());
    query.prepare("SELECT indexname FROM pg_indexes WHERE tablename = ?");
    query.addBindValue(tableName);
    QStringList indexes;
    if (!ExecuteQuery(&query))
    {
        while (query.next())
            indexes.append(query.value(0).toString());
    }

    return indexes.join(";");
}

QString TableObjectsModel::getTriggersPostgres() const
{
    QSqlQuery query(m_pConnection->db());
    query.prepare(
        "SELECT distinct trigger_name FROM information_schema.triggers "
        "WHERE event_object_table = ?");

    query.addBindValue(tableName);
    QStringList triggers;
    if (!ExecuteQuery(&query))
    {
        while (query.next())
            triggers.append(query.value(0).toString());
    }

    return triggers.join(";");
}

QString TableObjectsModel::getConstraintsPostgres() const
{
    QSqlQuery query(m_pConnection->db());
    query.prepare(
        "/*@ DisConv */SELECT constraint_name FROM information_schema.table_constraints "
        "WHERE table_name = ? AND constraint_type IN ('PRIMARY KEY', 'FOREIGN KEY', 'UNIQUE')");
    query.addBindValue(tableName);

    QStringList constraints;
    if (!ExecuteQuery(&query))
    {
        while (query.next())
            constraints.append(query.value(0).toString());
    }

    return constraints.join(";");
}

QString TableObjectsModel::getSequencesPostgres() const
{
    QSqlQuery query(m_pConnection->db());
    query.prepare(
        "SELECT sequence_name FROM information_schema.sequences "
        "WHERE sequence_name LIKE ? || '_%'");
    query.addBindValue(tableName);
    QStringList sequences;
    if (!ExecuteQuery(&query))
    {
        while (query.next())
            sequences.append(query.value(0).toString());
    }

    return sequences.join(";");
}

QString TableObjectsModel::getIndexDefinitionPostgres(const QString &indexName) const
{
    QSqlQuery query(m_pConnection->db());
    query.prepare(
        "SELECT indexdef FROM pg_indexes WHERE indexname = ?");
    query.addBindValue(indexName);

    if (!ExecuteQuery(&query) && query.next())
        return query.value(0).toString();

    return QString("Не удалось получить определение индекса %1").arg(indexName);
}

QString TableObjectsModel::getTriggerDefinitionPostgres(const QString &triggerName) const
{
    QSqlQuery query(m_pConnection->db());

    // Получаем определение триггера и информацию о функции
    query.prepare(
                "SELECT pg_get_triggerdef(t.oid) as trigger_def, "
                "       pg_get_functiondef(t.tgfoid) as function_def "
                "FROM pg_trigger t "
                "WHERE t.tgname = ?");
    query.addBindValue(triggerName);

    if (ExecuteQuery(&query))
    {
        qWarning() << "Ошибка выполнения запроса:" << query.lastError().text();
        return QString("Не удалось получить определение триггера %1").arg(triggerName);
    }

    if (query.next())
    {
        QString triggerDef = query.value("trigger_def").toString().trimmed();
        QString functionDef = query.value("function_def").toString().trimmed();

        return QString("%1\n/\n\n%2;\n/")
                .arg(functionDef)
                .arg(triggerDef);
    }

    return QString("Триггер %1 не найден").arg(triggerName);
}

QString TableObjectsModel::getConstraintDefinitionPostgres(const QString &constraintName) const
{
    QSqlQuery query(m_pConnection->db());
    query.prepare(
                "/*@ DisConv */SELECT pg_get_constraintdef(c.oid, true) AS definition "
                "FROM pg_constraint c "
                "JOIN pg_namespace n ON n.oid = c.connamespace "
                "WHERE c.conname = ? AND n.nspname NOT IN ('pg_catalog', 'information_schema')"
                );
    query.addBindValue(constraintName);

    if (!ExecuteQuery(&query) && query.next()) {
        return query.value("definition").toString();
    }

    return QString("Не удалось получить определение ограничения %1").arg(constraintName);
}

QString TableObjectsModel::getSequenceDefinitionPostgres(const QString &sequenceName) const
{
    QSqlQuery query(m_pConnection->db());
    query.prepare(
            "/*@ DisConv */SELECT 'CREATE SEQUENCE ' || sequence_name || E'\n' || "
            "'START ' || start_value || E'\n' || "
            "'INCREMENT ' || increment || E'\n' || "
            "'MAXVALUE ' || maximum_value || E'\n' || "
            "'MINVALUE ' || minimum_value || E'\n' || "
            "CASE WHEN cycle_option = 'YES' THEN 'CYCLE;' ELSE 'NO CYCLE;' END "
            "FROM information_schema.sequences WHERE sequence_name = ?");

    query.addBindValue(sequenceName);
    if (!ExecuteQuery(&query) && query.next())
        return query.value(0).toString();

    return QString("Не удалось получить определение последовательности %1").arg(sequenceName);
}

// Методы для Oracle

QString TableObjectsModel::getTableDefinitionOracle() const
{
    QSqlQuery query(m_pConnection->db());
    query.prepare(
        "SELECT DBMS_METADATA.GET_DDL('TABLE', :table_name) FROM dual");

    query.bindValue(":table_name", tableName.toUpper());
    if (!ExecuteQuery(&query) && query.next())
        return query.value(0).toString();

    return QString("Не удалось получить определение таблицы %1").arg(tableName);
}

QString TableObjectsModel::getIndexesOracle() const
{
    QSqlQuery query(m_pConnection->db());
    query.prepare(
        "SELECT index_name FROM all_indexes WHERE table_name = :table_name and OWNER = :owner");

    query.bindValue(":table_name", tableName.toUpper());
    query.bindValue(":owner",  m_pConnection->user());

    QStringList indexes;
    if (!ExecuteQuery(&query))
    {
        while (query.next())
            indexes.append(query.value(0).toString());
    }

    return indexes.join(";");
}

QString TableObjectsModel::getTriggersOracle() const
{
    QSqlQuery query(m_pConnection->db());
    query.prepare(
        "SELECT trigger_name FROM all_triggers WHERE table_name = :table_name and OWNER = :owner");
    query.bindValue(":table_name", tableName.toUpper());
    query.bindValue(":owner",  m_pConnection->user());

    QStringList triggers;
    if (!ExecuteQuery(&query))
    {
        while (query.next())
            triggers.append(query.value(0).toString());
    }

    return triggers.join(";");
}

QString TableObjectsModel::getConstraintsOracle() const
{
    QSqlQuery query(m_pConnection->db());
    query.prepare(
        "SELECT constraint_name FROM all_constraints "
        "WHERE table_name = :table_name AND constraint_type IN ('P', 'R', 'U', 'C') and OWNER = :owner");
    query.bindValue(":table_name", tableName.toUpper());
    query.bindValue(":owner",  m_pConnection->user());
    QStringList constraints;
    if (!ExecuteQuery(&query))
    {
        while (query.next())
            constraints.append(query.value(0).toString());
    }

    return constraints.join(";");
}

QString TableObjectsModel::getSequencesOracle() const
{
    QSqlQuery query(m_pConnection->db());
    query.prepare(
        "SELECT sequence_name FROM all_sequences "
        "WHERE sequence_name LIKE :table_prefix || '%' and sequence_owner = :owner");

    query.bindValue(":table_prefix", tableName.toUpper() + "_");
    query.bindValue(":owner",  m_pConnection->user());
    QStringList sequences;
    if (!ExecuteQuery(&query))
    {
        while (query.next())
            sequences.append(query.value(0).toString().simplified());
    }

    return sequences.join(";");
}

QString TableObjectsModel::getIndexDefinitionOracle(const QString &indexName) const
{
    QSqlQuery query(m_pConnection->db());
    query.prepare(
        "SELECT trim(to_char(DBMS_METADATA.GET_DDL('INDEX', :index_name, :owner))) FROM dual");

    query.bindValue(":index_name", indexName);
    query.bindValue(":owner", m_pConnection->user());

    if (!ExecuteQuery(&query) && query.next())
        return query.value(0).toString().simplified();

    return QString("Не удалось получить определение индекса %1").arg(indexName);
}

QString TableObjectsModel::getTriggerDefinitionOracle(const QString &triggerName) const
{
    QSqlQuery query(m_pConnection->db());
    query.prepare(
        "SELECT to_char(DBMS_METADATA.GET_DDL('TRIGGER', :trigger_name, :owner)) FROM dual");

    query.bindValue(":trigger_name", triggerName);
    query.bindValue(":owner", m_pConnection->user());

    if (query.exec() && query.next())
        return query.value(0).toString().trimmed();

    return QString("Не удалось получить определение триггера %1").arg(triggerName);
}

QString TableObjectsModel::getConstraintDefinitionOracle(const QString &constraintName) const
{
    QSqlQuery query(m_pConnection->db());
    query.prepare(
        "SELECT to_char(DBMS_METADATA.GET_DDL('CONSTRAINT', :constraint_name, :owner)) FROM dual");

    query.bindValue(":constraint_name", constraintName);
    query.bindValue(":owner", m_pConnection->user());

    if (!ExecuteQuery(&query) && query.next())
        return query.value(0).toString().simplified();

    return QString("Не удалось получить определение ограничения %1").arg(constraintName);
}

QString TableObjectsModel::getSequenceDefinitionOracle(const QString &sequenceName) const
{
    QSqlQuery query(m_pConnection->db());
    query.prepare(
        "SELECT to_char(DBMS_METADATA.GET_DDL('SEQUENCE', :sequence_name, :owner)) FROM dual");

    query.bindValue(":sequence_name", sequenceName);
    query.bindValue(":owner", m_pConnection->user());

    if (!ExecuteQuery(&query) && query.next())
        return query.value(0).toString().simplified();

    return QString("Не удалось получить определение последовательности %1").arg(sequenceName);
}
