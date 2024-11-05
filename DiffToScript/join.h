#ifndef JOIN_H
#define JOIN_H

#include "tablelinks.h"
#include "scripttable.h"
#include "dattable.h"
#include <map>

struct JoinTable;
struct JoinIndex;

struct JoinIndex: public QMap<int, QVector<int>>
{
    void append(int p, int c);
};

struct Join;
struct JoinListIterator;

struct JoinList: public QList<Join*>
{
public:
    JoinList::iterator firstChild(JoinTable* joinTable);
};


struct Join
{
public:
    Join(JoinTable* parent, JoinTable* child, const TableLinks& parentTableLinks);
    JoinIndex indexUpToDown; //Звязи записей. От родительской к дочерней.
    JoinIndex indexDownToUp; //Звязи записей. От дочерней к родительской.
    QVector<int> childForeignFields; //Номера внешних полей для дочерней таблицы
    QVector<int> parentForeignFields; //Номера внешних полей для родительсой таблицы
    JoinTable* parent;
    JoinTable* child;
    QStringList getValuesByIndex(const QVector<int>& fieldIndexes, const DatRecord& record);
private:
    bool makeIndex(ScriptTable* parent, ScriptTable* child, const TableLinks& parentTableLinks);
};


struct JoinTable
{
    ScriptTable* scriptTable;
    QVector<bool> processedRecords;
    //DatTable* datTable;
    JoinList joinList;
    QStringList keyFields;
    JoinTable(ScriptTable* datTable, const TableLinks& tableLinks);
    void setKeyFields(const TableLinks& tableLinks);
    Join* getChildJoin() const;
    Join* getParentJoin() const;
};

struct JoinTables
{
    QList<JoinTable*> joinTableList;
    QList<Join*> joins;
    QList<TableLinks> tableLinksList;
    ~JoinTables();
    void add(ScriptTable* datTable, const TableLinks& tableLinks);
    void build();
    JoinTable* tableByName(QString name);
    bool hasJoin(QString parent, QString child);
    JoinTable *getRoot();    
};

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
