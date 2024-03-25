#ifndef SQLSCRIPTINSERT_H
#define SQLSCRIPTINSERT_H

#include <dattable.h>
#include "join.h"
#include "tablelinks.h"

struct Statement
{
    virtual void buildStatement() = 0;
};

class SqlScriptInsert
{
public:
    Join* _join;
    void build(QTextStream& os);
    SqlScriptInsert();
};

#endif // SQLSCRIPTINSERT_H
