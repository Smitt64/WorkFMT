#include "join.h"
#include "difflogging.h"

int indexOfTable(const QVector<TableLinks>& tableLinks, QString name)
{
    for (int i = 0; i < tableLinks.count(); ++i)
    {
        if (tableLinks[i].tableName.toLower() == name)
            return i;
    }

    return -1;
}

int indexOfLink(const Links& links, QString parentName)
{
    for (int i = 0; i < links.count(); ++i)
    {
        if (links[i]->tableName.toLower() == parentName)
            return i;
    }

    return -1;
}

struct ParentChild
{
    int parent;
    int child;
};

// ----------------------------------------------------------------------------

JoinIndex::JoinIndex(QObject* parent) :
    QObject(parent)
{

}

JoinIndex::JoinIndex(const JoinIndex& other)
    : QObject(other.parent()),
      IntegerVectorMap(other)
{

}

JoinIndex::JoinIndex(JoinIndex&& other) noexcept
    : QObject(other.parent()),
      IntegerVectorMap(std::move(other))
{

}

JoinIndex& JoinIndex::operator=(const JoinIndex& other)
{
    if (this != &other)
    {
        IntegerVectorMap::operator=(other);
        setParent(other.parent());
    }

    return *this;
}

JoinIndex& JoinIndex::operator=(JoinIndex&& other) noexcept
{
    if (this != &other)
    {
        IntegerVectorMap::operator=(std::move(other));
        setParent(other.parent());
    }

    return *this;
}

bool JoinIndex::hasValues(const int &id)
{
    return IntegerVectorMap::contains(id);
}

QVariantList JoinIndex::values(const int &id)
{
    QVariantList list;
    const QVector<int> &vec = IntegerVectorMap::value(id);

    for (auto item : vec)
        list.append(item);

    return list;
}

int JoinIndex::value(const int &id, const int &index)
{
    if (hasValues(id))
    {
        const QVector<int> &vec = IntegerVectorMap::value(id);
        if (index >= 0 && index < vec.size())
            return vec[index];
    }

    return -1;
}

void JoinIndex::append(int p, int c)
{
    if (find(p) == end())
        (*this)[p] = QVector<int>();
    (*this)[p].push_back(c);
}

QVariantList JoinIndex::getIds() const
{
    QVariantList list;
    QList<int> _keys = IntegerVectorMap::keys();

    for (auto item : _keys)
        list.append(item);

    return list;
}

// ----------------------------------------------------------------------------

QString findParentTable(const QVector<TableLinks>& tableLinks)
{
    for (const TableLinks& tableLink: tableLinks)
    {
        if (tableLink.links.count() == 0)
            return tableLink.tableName;
    }
    return "";
}

QString findChildTable(QString parentName, const QVector<TableLinks>& tableLinks)
{
    for (const TableLinks& tableLink: tableLinks)
    {
        for (const Link *link: tableLink.links)
        {
            if (link->tableName.toLower() == parentName.toLower())
                return tableLink.tableName;
        }
    }

    return "";
}

const ScriptTable& findTable(QString name, const QVector<ScriptTable>& datTables)
{
    for (const ScriptTable& tab: datTables)
    {
        if (tab.name.toLower() == name.toLower())
            return tab;
    }

    Q_ASSERT_X(false, "join.h, findTable. table not found", name.toStdString().c_str());
}


Join::Join(JoinTable* parent, JoinTable* child, const TableLinks &parentTableLinks)
{
    this->parent = nullptr;
    this->child  = nullptr;

    for (const Link *link: parentTableLinks.links)
    {
        if (link->tableName.toLower() == child->scriptTable->name.toLower())
        {
            this->child = child;
            child->joinList.append(this);
            qCInfo(logJoin) << "Child table =" << child->scriptTable->name;

        }
    }

    if (parentTableLinks.tableName.toLower() == parent->scriptTable->name.toLower())
    {
        this->parent = parent;
        parent->joinList.append(this);
        qCInfo(logJoin) << "Parent table =" << parent->scriptTable->name;
    }

    bool b = (this->parent != nullptr && this->child != nullptr);
    if(!b)
        qCWarning(logJoin) << "Join::Join", "Parent or child tables are not defined.";

    Q_ASSERT_X(b , "Join::Join", "Parent or child tables are not defined.");

    qCInfo(logJoin) << "Created join " << parent->scriptTable->name << "-" << child->scriptTable->name;

    makeIndex(parent->scriptTable, child->scriptTable, parentTableLinks);
}

bool Join::makeIndex(ScriptTable* parent, ScriptTable* child, const TableLinks &parentTableLinks)
{
    qCInfo(logJoin) << "Creating index for " << parent->name << "-" << child->name;

    int linkIndex = parentTableLinks.links.getIndexByChildTableName(child->name);
    if (linkIndex == -1)
    {
        qCInfo(logJoin) << "Join doesn't have foreign links. Parent table =" << parent->name << ", child table = " << child->name;
        return true;
    }

    //Запоминание номеров внешних полей для родительской и дочерней таблиц
    for (const LinkField *linkField: parentTableLinks.links[linkIndex]->fields)
    {
        //Опреление внешнего индекса по названию поля и запоминание индекса
        parentForeignFields.append(parent->fields.indexByFieldName(linkField->parentField));
        if (parentForeignFields.back() == -1)
        {
            qCWarning(logJoin) << "Not found field by name: " << linkField->parentField;
            return false;
        }

        childForeignFields.append(child->fields.indexByFieldName(linkField->field));
        if (childForeignFields.back() == -1)
        {
            qCWarning(logJoin) << "Not found field by name: " << linkField->field;
            return false;
        }
    }

    //Индекс для родительской и дочерней таблиц
    for(int parentIndx = 0; parentIndx < parent->records.count(); ++parentIndx)
    {
        QStringList parentValues = getValuesByIndex(parentForeignFields, parent->records[parentIndx]);
        for (int childIndx = 0; childIndx < child->records.count(); ++childIndx)
        {
            QStringList childValues = getValuesByIndex(childForeignFields, child->records[childIndx]);
            if (parentValues == childValues)
            {
                //
                if (true/*parent->records[parentIndx].lineUpdateType != lutOld*/)
                {
                    indexUpToDown.append(parentIndx, childIndx);
                    indexDownToUp.append(childIndx, parentIndx);
                    qCInfo(logJoin)
                            << "Index created"
                            << ". Parent:" << parent->records[parentIndx]->lineTypeStr() << parent->records[parentIndx]->lineUpdateTypeStr()
                            << ". Child:"  << child->records[childIndx]->lineTypeStr()  << child->records[childIndx]->lineUpdateTypeStr()
                            << ". ForeignKey =" << childValues;
                }
                else
                {
                    qCInfo(logJoin)
                            << "Index skipped"
                            << ". Parent:" << parent->records[parentIndx]->lineTypeStr() << parent->records[parentIndx]->lineUpdateTypeStr()
                            << ". Child:"  << child->records[childIndx]->lineTypeStr()  << child->records[childIndx]->lineUpdateTypeStr()
                            << ". ForeignKey =" << childValues;
                }
            }
        }
    }
    qCInfo(logJoin) << "Index count = " << indexUpToDown.size();

    return true;
}

//Возварщает значения для полей с номером fieldIndexes
QStringList Join::getValuesByIndex(const QVector<int>& fieldIndexes, const DatRecord *record)
{
    QStringList values;

    for (int index: fieldIndexes)
        values.append(record->values.at(index));

    return values;
}

QStringList Join::getValuesByIndex(const QVariantList& fieldIndexes, const DatRecord *record)
{
    QStringList values;

    for (const QVariant &index: fieldIndexes)
    {
        bool ok = false;
        int index_ = index.toInt(&ok);

        if (ok)
            values.append(record->values.at(index_));
    }

    return values;
}

QObject *Join::getParent() const
{
    return parent;
}

QObject *Join::getChild() const
{
    return child;
}

QVariantList Join::getChildForeignFields() const
{
    QVariantList list;
    list.reserve(childForeignFields.size());

    for (const auto& item : qAsConst(childForeignFields))
        list.append(QVariant::fromValue(item));

    return list;
}

QVariantList Join::getParentForeignFields() const
{
    QVariantList list;
    list.reserve(parentForeignFields.size());

    for (const auto& item : qAsConst(parentForeignFields))
        list.append(QVariant::fromValue(item));

    return list;
}

JoinIndex *Join::getIndexUpToDown()
{
    return &indexUpToDown;
}

JoinIndex *Join::getIndexDownToUp()
{
    return &indexDownToUp;
}

// ----------------------------------------------------------------------------

JoinTableIterator JoinTableIterator::getChildTable()
{
    for (Join* join: joinTable->joinList)
        if (join->parent->scriptTable->name.toLower() == joinTable->scriptTable->name.toLower())
            return JoinTableIterator(join->child);
    return JoinTableIterator();
}

bool operator==(const JoinTableIterator& a, const JoinTableIterator& b)
{
    return (a.joinTable == b.joinTable);
}

bool operator!=(const JoinTableIterator& a, const JoinTableIterator& b)
{
    return !(a == b);
}

// ----------------------------------------------------------------------------

JoinTable::JoinTable(ScriptTable *datTable, const TableLinks &tableLinks)
{
    setKeyFields(tableLinks);
    this->scriptTable = datTable;
    this->processedRecords.resize(this->scriptTable->records.count());
    this->processedRecords.fill(false);
    qCInfo(logJoinTable) << "Created JoinTable " << this->scriptTable->name;
}

void JoinTable::setKeyFields(const TableLinks &tableLinks)
{
    keyFields = tableLinks.index;
}

Join *JoinTable::getChildJoin() const
{
    for (JoinList::const_iterator it = joinList.begin(); it != joinList.end(); ++it)
    {
        if ((*it)->parent->scriptTable->name.toLower() == scriptTable->name.toLower())
            return *it;
    }
    return nullptr;
}


Join *JoinTable::getParentJoin() const
{
    for (int i = 0; i < joinList.size(); ++i)
        if (joinList[i]->child->scriptTable->name.toLower() == scriptTable->name.toLower())
            return joinList[i];
    return nullptr;
}

BoolVector *JoinTable::getProcessedRecords()
{
    return &processedRecords;
}

JoinList *JoinTable::getJoinList()
{
    return &joinList;
}

const QStringList &JoinTable::getKeyFields() const
{
    return keyFields;
}

ScriptTable *JoinTable::getScriptTable()
{
    return scriptTable;
}

// ----------------------------------------------------------------------------

JoinTables::~JoinTables()
{
    // Удаляется деструктором объекта IterableObject
    /*for (int i = 0; i < joins.count(); ++i)
        delete joins[i];*/
}

void JoinTables::add(ScriptTable *datTable, TableLinks *tableLinks)
{
    joinTableList.append(new JoinTable(datTable, *tableLinks));
    tableLinksList.append(tableLinks);

    qCInfo(logJoinTables) << "Added table" << datTable->name;
    build();
}

void JoinTables::build()
{
    qCInfo(logJoinTables) << "Rebuild table joines. JoinTable count =" << joinTableList.count();
    for (TableLinks *tableLinks: qAsConst(tableLinksList))
    {
        QString parent = tableLinks->tableName;
        for (const Link *link: tableLinks->links)
        {
            QString child = link->tableName;
            JoinTable* parentJoinTable = tableByName(parent);
            JoinTable* childJoinTable = tableByName(child);
            if (parentJoinTable != nullptr && childJoinTable != nullptr)
            {
                if (!hasJoin(parent, child))
                    joins.append(new Join(parentJoinTable, childJoinTable, *tableLinks));
            }
        }
    }
}

JoinTable *JoinTables::tableByName(QString name)
{
    for (int i = 0; i < joinTableList.count(); ++i)
    {
        if (joinTableList[i]->scriptTable->name.toLower() == name.toLower())
            return joinTableList[i];
    }

    return nullptr;
}

bool JoinTables::hasJoin(QString parent, QString child)
{
    for (Join* join: qAsConst(joins))
    {
        if (join->child->scriptTable->name.toLower() == child.toLower() && join->parent->scriptTable->name.toLower() == parent.toLower())
            return true;
    }

    return false;
}

JoinTable *JoinTables::getRoot()
{
    JoinTable *table= nullptr;
    //Join *parentJoin= nullptr;

    if (!joinTableList.count())
        return nullptr;

    table = joinTableList[0];
    //parentJoin = table->getParentJoin();

    while (table->getParentJoin() != nullptr)
        table = table->getParentJoin()->parent;

    return table;
}

JoinTableList *JoinTables::getJoinTableList()
{
    return &joinTableList;
}

JoinList *JoinTables::getJoins()
{
    return &joins;
}

TableLinksList *JoinTables::getTableLinksList()
{
    return &tableLinksList;
}

// ---------------------------------------------------------------------------------------

JoinListIterator::JoinListIterator()
    :JoinListIterator(nullptr, JoinList::iterator())
{

}


JoinListIterator::JoinListIterator(JoinTable *joinTable, JoinList::iterator first)
    : itChild(first),
      _joinTable(joinTable)
{
}

JoinListIterator JoinListIterator::nextChild()
{
    if (this->_joinTable == nullptr)
        return JoinListIterator();

    ++itChild;
    for (; itChild != _joinTable->joinList.end(); ++itChild)
        if ((*itChild)->parent->scriptTable->name.toLower() == _joinTable->scriptTable->name.toLower())
            break;

    return JoinListIterator(_joinTable, itChild);
}

JoinListIterator &JoinListIterator::operator=(const JoinListIterator &it)
{
    itChild = it.itChild;
    _joinTable = it._joinTable;
    return *this;
}

JoinList::iterator JoinList::firstChild(JoinTable* joinTable)
{
    JoinList::iterator it = begin();
    for (; it != end(); ++it)
        if ((*it)->parent->scriptTable->name.toLower() == joinTable->scriptTable->name.toLower())
            break;
    return it;
}

// ----------------------------------------------------------------------------

