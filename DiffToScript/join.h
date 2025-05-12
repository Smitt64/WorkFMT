#ifndef JOIN_H
#define JOIN_H

#include "tablelinks.h"
#include "scripttable.h"
#include "dattable.h"
#include "rslmodule/iterableobjectbase.h"
#include <map>

class JoinTable;
class JoinIndex;

using IntegerVectorMap = QMap<int, QVector<int>>;
class JoinIndex: public QObject, public IntegerVectorMap
{
    Q_OBJECT
    Q_PROPERTY(QVariantList ids READ getIds CONSTANT)
public:
    JoinIndex(QObject* parent = nullptr);
    JoinIndex(const JoinIndex& other);
    JoinIndex(JoinIndex&& other) noexcept;

    JoinIndex& operator=(const JoinIndex& other);
    JoinIndex& operator=(JoinIndex&& other) noexcept;

    Q_INVOKABLE bool hasValues(const int &id);
    Q_INVOKABLE QVariantList values(const int &id);
    Q_INVOKABLE int value(const int &id, const int &index);

    void append(int p, int c);
    QVariantList getIds() const;
};

Q_DECLARE_OPAQUE_POINTER(JoinIndex)

class Join;
struct JoinListIterator;

class JoinList: public ListIterableObject<Join*, CachedObjectDeleter>
{
    Q_OBJECT
public:
    JoinList::iterator firstChild(JoinTable* joinTable);
};

Q_DECLARE_OPAQUE_POINTER(JoinList)

class Join : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QObject *parent READ getParent CONSTANT)
    Q_PROPERTY(QObject *child READ getChild CONSTANT)
    Q_PROPERTY(QVariantList childForeignFields READ getChildForeignFields CONSTANT)
    Q_PROPERTY(QVariantList parentForeignFields READ getParentForeignFields CONSTANT)
    Q_PROPERTY(JoinIndex *indexUpToDown READ getIndexUpToDown CONSTANT)
    Q_PROPERTY(JoinIndex *indexDownToUp READ getIndexDownToUp CONSTANT)
public:
    Join(JoinTable* parent, JoinTable* child, const TableLinks& parentTableLinks);
    virtual ~Join() = default;

    JoinIndex indexUpToDown; //Cвязи записей. От родительской к дочерней.
    JoinIndex indexDownToUp; //Cвязи записей. От дочерней к родительской.

    QVector<int> childForeignFields; //Номера внешних полей для дочерней таблицы
    QVector<int> parentForeignFields; //Номера внешних полей для родительсой таблицы

    QVariantList getChildForeignFields() const;
    QVariantList getParentForeignFields() const;

    JoinTable* parent;
    JoinTable* child;

    QObject *getParent() const;
    QObject *getChild() const;

    JoinIndex *getIndexUpToDown();
    JoinIndex *getIndexDownToUp();

    QStringList getValuesByIndex(const QVector<int>& fieldIndexes, const DatRecord *record);
    Q_INVOKABLE QStringList getValuesByIndex(const QVariantList& fieldIndexes, const DatRecord *record);

private:
    bool makeIndex(ScriptTable* parent, ScriptTable* child, const TableLinks& parentTableLinks);
};

Q_DECLARE_OPAQUE_POINTER(Join)

class JoinTable : public QObject
{
    Q_OBJECT
    Q_PROPERTY(JoinList *joinList READ getJoinList CONSTANT)
    Q_PROPERTY(QStringList keyFields READ getKeyFields CONSTANT)
    Q_PROPERTY(ScriptTable *scriptTable READ getScriptTable CONSTANT)
public:
    ScriptTable* scriptTable;
    QVector<bool> processedRecords;
    //DatTable* datTable;

    JoinList joinList;
    QStringList keyFields;

    JoinTable(ScriptTable* datTable, const TableLinks& tableLinks);

    void setKeyFields(const TableLinks& tableLinks);
    Join* getChildJoin() const;
    Join* getParentJoin() const;

    JoinList *getJoinList();
    ScriptTable *getScriptTable();
    const QStringList &getKeyFields() const;
};

Q_DECLARE_OPAQUE_POINTER(JoinTable)

class JoinTableList : public ListIterableObject<JoinTable*, CachedObjectDeleter>
{
    Q_OBJECT
public:
    JoinTableList() = default;
    virtual ~JoinTableList() = default;
};

Q_DECLARE_OPAQUE_POINTER(JoinTableList)

class JoinTables : public QObject
{
    Q_OBJECT
    Q_PROPERTY(JoinTableList *joinTableList READ getJoinTableList CONSTANT)
    Q_PROPERTY(JoinList *joins READ getJoins CONSTANT)
    Q_PROPERTY(TableLinksList *tableLinksList READ getTableLinksList CONSTANT)
public:
    JoinTableList joinTableList;
    JoinList joins;
    TableLinksList tableLinksList;

    JoinTables() = default;
    virtual ~JoinTables();

    void add(ScriptTable* datTable, TableLinks *tableLinks);
    void build();

    Q_INVOKABLE JoinTable* tableByName(QString name);
    Q_INVOKABLE bool hasJoin(QString parent, QString child);
    Q_INVOKABLE JoinTable *getRoot();

    JoinTableList *getJoinTableList();
    JoinList *getJoins();
    TableLinksList *getTableLinksList();
};

Q_DECLARE_OPAQUE_POINTER(JoinTables)

struct JoinListIterator
{
    JoinListIterator(JoinTable* joinTable, JoinList::iterator first);
    JoinListIterator();
    JoinListIterator nextChild(); // j
    JoinListIterator& operator=(const JoinListIterator& it);
    JoinList::iterator itChild;
private:
    JoinTable* _joinTable;
};

struct JoinTableIterator
{
    JoinTableIterator():joinTable(nullptr){}
    JoinTableIterator(const JoinTable* joinTable):joinTable(joinTable){}
    const JoinTable* joinTable;
    JoinTableIterator getChildTable();
    const JoinTable* operator->() { return joinTable; }
    const JoinTable* ptr() {return joinTable;}
};

bool operator==(const JoinTableIterator& a, const JoinTableIterator& b);
bool operator!=(const JoinTableIterator& a, const JoinTableIterator& b);

#endif // JOIN_H
