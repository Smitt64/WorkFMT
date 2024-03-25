#ifndef SQLSCRIPTDELETE_H
#define SQLSCRIPTDELETE_H

#include "join.h"

class SqlScriptDelete
{
public:
    SqlScriptDelete();
    Join* _join;
    void build(QTextStream& os);
    void buildDeleteScript();
};

#endif // SQLSCRIPTDELETE_H
