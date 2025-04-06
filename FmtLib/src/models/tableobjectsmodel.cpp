#include "tableobjectsmodel.h"
#include "connectioninfo.h"
#include "fmtcore.h"
#include <QSqlQuery>
#include <QSqlError>
#include <QIcon>
#include <QApplication>
#include <QStyle>
#include <QRegularExpression>

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

    if (item->type == RootItem && role == Qt::DisplayRole)
        return item->name;

    if (role == Qt::DisplayRole)
        return item->name;

    else if (role == Qt::CheckStateRole && (item->type != RootItem && item->type != CategoryItem))
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
    else if (role == Qt::DecorationRole)
    {
        QStyle *style = QApplication::style();

        switch(item->type)
        {
        case RootItem: return QIcon(":/table");
        case CategoryItem: return style->standardIcon(QStyle::SP_DirIcon);
        case IndexItem: return QIcon(":/index");
        case TriggerItem: return QIcon(":/trigger");
        case ConstraintItem: return QIcon(":/constraint");
        case SequenceItem: return QIcon(":/sequence");
        default: return QVariant();
        }
    }
    else if (role == Qt::ToolTipRole)
    {
        return QString("%1\nType: %2").arg(item->name).arg(item->objectTypeString());
    }

    return QVariant();
}

QString TableObjectsModel::getObjectType(const QModelIndex &index) const
{
    if (!index.isValid())
        return "Table";

    TreeNode *item = static_cast<TreeNode*>(index.internalPointer());
    return item->objectTypeString();
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

    // Если запрашивается индекс корневого элемента
    if (!parent.isValid())
        return createIndex(row, column, rootItem);

    TreeNode *parentItem = static_cast<TreeNode*>(parent.internalPointer());

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

    // Если parentItem == nullptr, значит это корневой элемент
    if (parentItem == nullptr || childItem == rootItem)
        return QModelIndex();

    // Если родитель - корневой элемент
    if (parentItem == rootItem)
        return createIndex(0, 0, rootItem);

    return createIndex(parentItem->parent->children.indexOf(parentItem), 0, parentItem);
}

int TableObjectsModel::rowCount(const QModelIndex &parent) const
{
    if (parent.column() > 0)
        return 0;

    // Если запрашивается количество строк корневого элемента
    if (!parent.isValid())
        return 1;  // Только один корневой элемент - сама таблица

    TreeNode *parentItem = static_cast<TreeNode*>(parent.internalPointer());
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

QString TableObjectsModel::getSqlForIndex(const QModelIndex &index, bool simplified) const
{
    if (!index.isValid())
        return rootItem->sql;

    TreeNode *item = static_cast<TreeNode*>(index.internalPointer());

    if (simplified)
    {
        if (item->type == IndexItem)
            return simplifyIndexDDL(item->sql);
        else if (item->type == SequenceItem)
            return simplifySequenceDDL(item->sql);
        else if (item->type == ConstraintItem)
            return simplifyConstraintDDL(item->sql);
        else if (item->type == TriggerItem)
            return simplifyTriggerDDL(item->sql);
    }

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
    //TreeNode* constraints = addCategory(rootItem, "Ограничения");
    TreeNode* sequences = addCategory(rootItem, "Последовательности");
    TreeNode* triggers = addCategory(rootItem, "Триггеры");

    // Заполняем категории объектами
    QStringList indexList = ((dbType == DbType::PostgreSQL) ?
        getIndexesPostgres() : getIndexesOracle()).split(";", Qt::SkipEmptyParts);
    addObjectsToCategory(indexes, indexList);

    QStringList triggerList = ((dbType == DbType::PostgreSQL) ?
        getTriggersPostgres() : getTriggersOracle()).split(";", Qt::SkipEmptyParts);
    addObjectsToCategory(triggers, triggerList);

   /* QStringList constraintList = ((dbType == DbType::PostgreSQL) ?
        getConstraintsPostgres() : getConstraintsOracle()).split(";", Qt::SkipEmptyParts);
    addObjectsToCategory(constraints, constraintList);*/

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
        TreeItemType objectType = ObjectItem;

        if (dbType == DbType::PostgreSQL)
        {
            if (category->name == "Индексы")
            {
                sql = getIndexDefinitionPostgres(objectName);
                objectType = IndexItem;
            }
            else if (category->name == "Ограничения")
            {
                sql = getConstraintDefinitionPostgres(objectName);
                objectType = ConstraintItem;
            }
            else if (category->name == "Последовательности")
            {
                sql = getSequenceDefinitionPostgres(objectName);
                objectType = SequenceItem;
            }
            else if (category->name == "Триггеры")
            {
                sql = getTriggerDefinitionPostgres(objectName);
                objectType = TriggerItem;
            }
        }
        else
        {
            if (category->name == "Индексы")
            {
                sql = getIndexDefinitionOracle(objectName);
                objectType = IndexItem;
            }
            else if (category->name == "Ограничения")
            {
                sql = getConstraintDefinitionOracle(objectName);
                objectType = ConstraintItem;
            }
            else if (category->name == "Последовательности")
            {
                sql = getSequenceDefinitionOracle(objectName);
                objectType = SequenceItem;
            }
            else if (category->name == "Триггеры")
            {
                sql = getTriggerDefinitionOracle(objectName);
                objectType = TriggerItem;
            }
        }

        TreeNode* object = new TreeNode
        {
                prefix + objectName,
                sql,
                objectType,  // Используем конкретный тип вместо ObjectItem
                Qt::Checked,
                category
    };

        category->children.append(object);
    }
}

QString TableObjectsModel::getTableDefinitionPostgres() const
{
    QSqlQuery query(m_pConnection->db());

    // 1. Проверяем, является ли таблица временной
    bool isTemporary = false;
    query.prepare(
                "SELECT relkind = 't' OR relpersistence = 't' "
                "FROM pg_class c "
                "JOIN pg_namespace n ON n.oid = c.relnamespace "
                "WHERE c.relname = ? AND n.nspname = current_schema()");
    query.addBindValue(tableName);

    if (!ExecuteQuery(&query) && query.next())
        isTemporary = query.value(0).toBool();

    // 2. Получаем определение таблицы
    query.prepare(R"(
            SELECT
                CASE
                    WHEN $1 THEN 'CREATE TEMPORARY TABLE '
                    ELSE 'CREATE TABLE '
                END || quote_ident(table_name) || ' (\n' ||
                string_agg(
                    '    ' || quote_ident(column_name) || ' ' ||
                    data_type ||
                    CASE WHEN character_maximum_length IS NOT NULL
                         THEN '(' || character_maximum_length || ')'
                         ELSE '' END ||
                    CASE WHEN is_nullable = 'NO' THEN ' NOT NULL' ELSE '' END ||
                    COALESCE(' DEFAULT ' || column_default, '') ||
                    COALESCE(' ' || extra_info, ''),
                    ',\n'
                ) || '\n);'
            FROM (
                SELECT
                    c.table_name,
                    c.column_name,
                    c.data_type,
                    c.character_maximum_length,
                    c.is_nullable,
                    c.column_default,
                    CASE
                        WHEN tc.constraint_type = 'PRIMARY KEY' THEN ' PRIMARY KEY'
                        WHEN tc.constraint_type = 'UNIQUE' THEN ' UNIQUE'
                        ELSE ''
                    END as extra_info
                FROM
                    information_schema.columns c
                LEFT JOIN
                    information_schema.key_column_usage kcu
                    ON c.table_name = kcu.table_name
                    AND c.column_name = kcu.column_name
                LEFT JOIN
                    information_schema.table_constraints tc
                    ON kcu.constraint_name = tc.constraint_name
                WHERE
                    c.table_name = ?
                ORDER BY
                    c.ordinal_position
            ) as cols
            GROUP BY
                table_name
        )");

    query.addBindValue(isTemporary);
    query.addBindValue(tableName);

    QString tableDef;
    if (!ExecuteQuery(&query) && query.next())
        tableDef = query.value(0).toString().replace("\\n", "\n");
    else
        tableDef = QString("Не удалось получить определение таблицы %1").arg(tableName);

    // 3. Получаем комментарии
    QStringList comments;

    // Комментарий к таблице
    query.prepare(
                "SELECT obj_description(c.oid) "
                "FROM pg_class c "
                "JOIN pg_namespace n ON n.oid = c.relnamespace "
                "WHERE c.relname = ? AND n.nspname = current_schema()");
    query.addBindValue(tableName);

    if (!ExecuteQuery(&query) && query.next())
    {
        QString comment = query.value(0).toString();
        if (!comment.isEmpty())
        {
            comments << QString("COMMENT ON TABLE %1 IS '%2';")
                        .arg(tableName)
                        .arg(comment.replace("'", "''"));
        }
    }

    // Комментарии к столбцам
    query.prepare(
                "SELECT a.attname, d.description "
                "FROM pg_description d "
                "JOIN pg_class c ON c.oid = d.objoid "
                "JOIN pg_attribute a ON a.attrelid = c.oid AND a.attnum = d.objsubid "
                "JOIN pg_namespace n ON n.oid = c.relnamespace "
                "WHERE c.relname = ? AND n.nspname = current_schema() AND d.description IS NOT NULL");
    query.addBindValue(tableName);

    if (!ExecuteQuery(&query))
    {
        while (query.next())
        {
            comments << QString("COMMENT ON COLUMN %1.%2 IS '%3';")
                        .arg(tableName)
                        .arg(query.value(0).toString())
                        .arg(query.value(1).toString().replace("'", "''"));
        }
    }

    // Собираем итоговый SQL
    if (!comments.isEmpty())
        tableDef += "\n\n" + comments.join("\n");

    return tableDef;
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
    QStringList result;

    // 1. Проверяем, является ли таблица временной (GLOBAL TEMPORARY)
    bool isTemporary = false;
    QString onCommitClause;
    query.prepare(
                "SELECT temporary, duration FROM all_tables "
                "WHERE table_name = :table_name AND owner = :owner");
    query.bindValue(":table_name", tableName.toUpper());
    query.bindValue(":owner", m_pConnection->user().toUpper());

    if (!ExecuteQuery(&query) && query.next())
    {
        isTemporary = (query.value(0).toString() == "Y");
        QString duration = query.value(1).toString();
        if (duration == "SYS$SESSION") {
            onCommitClause = " ON COMMIT PRESERVE ROWS";
        } else if (duration == "SYS$TRANSACTION") {
            onCommitClause = " ON COMMIT DELETE ROWS";
        }
    }

    // 2. Получаем столбцы таблицы (исправленный запрос без temporary)
    query.prepare(
                "SELECT column_name, data_type, data_length, data_precision, data_scale, "
                "       nullable, data_default "
                "FROM all_tab_columns "
                "WHERE table_name = :table_name AND owner = :owner "
                "ORDER BY column_id");
    query.bindValue(":table_name", tableName.toUpper());
    query.bindValue(":owner", m_pConnection->user().toUpper());

    if (ExecuteQuery(&query)) {
        return QString("Ошибка при получении столбцов таблицы %1").arg(tableName);
    }

    QStringList columns;
    while (query.next())
    {
        QString columnName = query.value("column_name").toString();
        QString dataType = query.value("data_type").toString();
        QString nullable = (query.value("nullable").toString() == "Y") ? "NULL" : "NOT NULL";
        QString defaultValue = query.value("data_default").toString().trimmed();

        // Обработка типов данных
        if (dataType == "NUMBER")
        {
            int precision = query.value("data_precision").toInt();
            int scale = query.value("data_scale").toInt();
            if (precision > 0) {
                dataType += QString("(%1%2)")
                        .arg(precision)
                        .arg(scale > 0 ? QString(",%1").arg(scale) : "");
            }
        }
        else if (dataType.contains("CHAR") || dataType == "RAW") {
            dataType += QString("(%1)").arg(query.value("data_length").toInt());
        }

        // Формируем определение столбца
        QString columnDef = QString("    %1 %2 %3").arg(columnName, dataType, nullable);

        if (!defaultValue.isEmpty())
        {
            defaultValue = defaultValue.replace(QRegularExpression("\\s+"), " ");
            if (defaultValue.contains("SYSDATE", Qt::CaseInsensitive)) {
                defaultValue = "SYSDATE";
            }
            columnDef += " DEFAULT " + defaultValue;
        }

        columns << columnDef;
    }

    // 3. Формируем CREATE TABLE с учетом временности
    QString createTable;
    if (isTemporary)
    {
        createTable = QString("CREATE GLOBAL TEMPORARY TABLE %1 (\n%2\n)%3;")
                .arg(tableName.toUpper())
                .arg(columns.join(",\n"))
                .arg(onCommitClause);
    } else
    {
        createTable = QString("CREATE TABLE %1 (\n%2\n)")
                .arg(tableName.toUpper())
                .arg(columns.join(",\n"));
    }
    result << createTable;

    // 4. Получаем комментарии (остальной код без изменений)
    query.prepare(
                "SELECT comments FROM all_tab_comments "
                "WHERE table_name = :table_name AND owner = :owner");
    query.bindValue(":table_name", tableName.toUpper());
    query.bindValue(":owner", m_pConnection->user().toUpper());

    if (!ExecuteQuery(&query) && query.next())
    {
        QString tableComment = query.value(0).toString().trimmed();
        if (!tableComment.isEmpty())
        {
            result << QString("COMMENT ON TABLE %1 IS '%2';")
                      .arg(tableName.toUpper())
                      .arg(tableComment.replace("'", "''"));
        }
    }

    query.prepare(
                "SELECT column_name, comments FROM all_col_comments "
                "WHERE table_name = :table_name AND owner = :owner AND comments IS NOT NULL");
    query.bindValue(":table_name", tableName.toUpper());
    query.bindValue(":owner", m_pConnection->user().toUpper());

    if (!ExecuteQuery(&query))
    {
        while (query.next())
        {
            result << QString("COMMENT ON COLUMN %1.%2 IS '%3';")
                      .arg(tableName.toUpper())
                      .arg(query.value(0).toString())
                      .arg(query.value(1).toString().replace("'", "''"));
        }
    }

    return result.join("\n\n");
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

QString TableObjectsModel::simplifyIndexDDL(const QString &originalDDL) const
{
    if (originalDDL.isEmpty())
        return originalDDL;

    QString result = originalDDL;

    // 1. Удаляем имя схемы перед индексом (сохраняем имя индекса)
    result.replace(QRegularExpression(
                       "(CREATE\\s+(?:UNIQUE\\s+)?INDEX\\s+)\"[^\"]+\"\\.(\"[^\"]+\")",
                       QRegularExpression::CaseInsensitiveOption), "\\1\\2");

    // 2. Удаляем имя схемы перед таблицей (сохраняем имя таблицы)
    result.replace(QRegularExpression(
                       "(ON\\s+)\"[^\"]+\"\\.(\"[^\"]+\")",
                       QRegularExpression::CaseInsensitiveOption), "\\1\\2");

    // 3. Удаляем параметры Oracle блоками
    QList<QRegularExpression> oracleParams = {
        QRegularExpression("PCTFREE\\s+\\d+", QRegularExpression::CaseInsensitiveOption),
        QRegularExpression("INITRANS\\s+\\d+", QRegularExpression::CaseInsensitiveOption),
        QRegularExpression("MAXTRANS\\s+\\d+", QRegularExpression::CaseInsensitiveOption),
        QRegularExpression("COMPUTE\\s+STATISTICS", QRegularExpression::CaseInsensitiveOption),
        QRegularExpression("STORAGE\\([^)]+\\)", QRegularExpression::CaseInsensitiveOption),
        QRegularExpression("TABLESPACE\\s+\"[^\"]+\"", QRegularExpression::CaseInsensitiveOption),
        QRegularExpression("TABLESPACE\\s+\\w+", QRegularExpression::CaseInsensitiveOption)
    };

    foreach (const QRegularExpression &regex, oracleParams)
        result.remove(regex);

    // 4. Удаляем всё после последней закрывающей скобки
    int lastParen = result.lastIndexOf(')');
    if (lastParen != -1)
        result = result.left(lastParen + 1);

    // 5. Удаляем точку с запятой в конце
    result = result.trimmed();
    if (result.endsWith(';'))
        result.chop(1);

    return result.trimmed();
}

QString TableObjectsModel::simplifySequenceDDL(const QString &originalDDL) const
{
    if (originalDDL.isEmpty())
        return originalDDL;

    QString result = originalDDL;

    // Удаление имени схемы перед последовательностью
    QRegularExpression sequenceSchemaRegex(
                "(CREATE\\s+SEQUENCE\\s+)(?:[\"]?[^\\s\"]+[\"]?\\s*\\.\\s*)([\"]?)([^\\s\"]+)([\"]?)",
                QRegularExpression::CaseInsensitiveOption);
    result.replace(sequenceSchemaRegex, "\\1\\2\\3\\4");

    // Для Oracle удаляем специфичные параметры
    if (dbType == DbType::Oracle)
    {
        result.remove(QRegularExpression(
                          "\\s+(TABLESPACE|STORAGE|CACHE|NOCACHE|ORDER|NOORDER|CYCLE|NOCYCLE)\\s+.+",
                          QRegularExpression::CaseInsensitiveOption));
    }

    return result.trimmed();
}

QString TableObjectsModel::simplifyConstraintDDL(const QString &originalDDL) const
{
    if (originalDDL.isEmpty())
        return originalDDL;

    QString result = originalDDL;

    // 1. Удаление имени схемы перед таблицей
    QRegularExpression tableSchemaRegex(
        "(ALTER\\s+TABLE\\s+)(?:[\"]?[^\\s\"]+[\"]?\\s*\\.\\s*)([\"]?)([^\\s\"]+)([\"]?\\s+)"
        "(ADD\\s+(?:CONSTRAINT\\s+)?)([\"]?)([^\\s\"]+)([\"]?)",
        QRegularExpression::CaseInsensitiveOption);
    result.replace(tableSchemaRegex, "\\1\\2\\3\\4\\5\\6\\7\\8");

    // 2. Удаление REFERENCES с именем схемы
    QRegularExpression refSchemaRegex(
        "(REFERENCES\\s+)(?:[\"]?[^\\s\"]+[\"]?\\s*\\.\\s*)([\"]?)([^\\s\"]+)([\"]?)",
        QRegularExpression::CaseInsensitiveOption);
    result.replace(refSchemaRegex, "\\1\\2\\3\\4");

    // 3. Удаление специфичных параметров
    if (dbType == DbType::Oracle)
    {
        result.remove(QRegularExpression(
            "\\s+(USING\\s+INDEX\\s+(TABLESPACE|STORAGE)\\s+.+)",
            QRegularExpression::CaseInsensitiveOption));
    }

    return result.trimmed();
}

QString TableObjectsModel::simplifyTriggerDDL(const QString &originalDDL) const
{
    if (originalDDL.isEmpty())
            return originalDDL;

        QString result = originalDDL;

        // 1. Удаление EDITIONABLE (если есть)
        result.remove("EDITIONABLE ");

        // 2. Удаление имени схемы перед триггером в CREATE OR REPLACE TRIGGER
        result.replace(QRegularExpression(
            "(CREATE\\s+OR\\s+REPLACE\\s+TRIGGER\\s+)(?:[\"]?[^\\s\"]+[\"]?\\s*\\.\\s*)([\"]?)([^\\s\"]+)([\"]?)",
            QRegularExpression::CaseInsensitiveOption), "CREATE OR REPLACE TRIGGER \\2\\3\\4");

        // 3. Удаление имени схемы перед триггером в ALTER TRIGGER, сохраняя ENABLE/DISABLE
        result.replace(QRegularExpression(
            "(ALTER\\s+TRIGGER\\s+)(?:[\"]?[^\\s\"]+[\"]?\\s*\\.\\s*)([\"]?)([^\\s\"]+)([\"]?)(\\s+(ENABLE|DISABLE))?",
            QRegularExpression::CaseInsensitiveOption), "ALTER TRIGGER \\2\\3\\4\\5");

        // 4. Удаление имени схемы перед таблицей в ON clause
        result.replace(QRegularExpression(
            "(ON\\s+)(?:[\"]?[^\\s\"]+[\"]?\\s*\\.\\s*)([\"]?)([^\\s\"]+)([\"]?)",
            QRegularExpression::CaseInsensitiveOption), "ON \\2\\3\\4");

        // 5. Очистка лишних переносов строк перед ALTER TRIGGER
        int alterPos = result.indexOf("ALTER TRIGGER");
        if (alterPos != -1)
        {
            // Находим позицию конца определения триггера (перед ALTER)
            int triggerEnd = alterPos - 1;
            while (triggerEnd >= 0 && (result[triggerEnd] == '\n' || result[triggerEnd] == '\r' || result[triggerEnd] == ' '))
                triggerEnd--;

            // Удаляем все переносы строк между концом триггера и ALTER TRIGGER
            result.remove(triggerEnd + 1, alterPos - triggerEnd - 1);

            // Вставляем один перенос строки и / перед ALTER TRIGGER
            result.insert(triggerEnd + 1, "\n/\n");
        }

        return result.trimmed();
}

TableObjectsInfoList TableObjectsModel::getCheckedObjectsWithSql(bool simplifiedSql) const
{
    TableObjectsInfoList result;

    // Добавляем RootItem (всегда первый элемент)
    QModelIndex rootIndex = index(0, 0);
    if (!rootIndex.isValid())
        return result;

    TreeNode *rootNode = static_cast<TreeNode*>(rootIndex.internalPointer());
    QString rootSql = rootNode->sql;
            //simplifiedSql ? simplifyTableDDL(rootNode->sql) : rootNode->sql;
    result.append({rootNode->name, RootItem, rootSql});

    // Рекурсивный обход дерева
    std::function<void(const QModelIndex&)> traverseTree;
    traverseTree = [&](const QModelIndex &parentIndex)
    {
        int childCount = rowCount(parentIndex);
        for (int i = 0; i < childCount; ++i)
        {
            QModelIndex childIndex = index(i, 0, parentIndex);
            TreeNode *item = static_cast<TreeNode*>(childIndex.internalPointer());

            // Берём только Index/Trigger/Sequence с галочкой
            bool isChecked = (item->checkState == Qt::Checked);
            bool isRelevantType = (item->type == IndexItem || item->type == TriggerItem || item->type == SequenceItem);

            if (isRelevantType && isChecked) {
                QString sql = getSqlForIndex(childIndex, simplifiedSql);
                result.append({item->name, item->type, sql});
            }

            traverseTree(childIndex); // Рекурсивно обходим детей
        }
    };

    traverseTree(rootIndex);
    return result;
}
