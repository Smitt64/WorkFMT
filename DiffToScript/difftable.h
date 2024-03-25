#ifndef DIFFTABLE_H
#define DIFFTABLE_H

#include "difffield.h"

#include <QList>

class DiffTable
{
public:
    DiffTable();
    QList<DiffField> fields;
    QList<QStringList> records;
};

#endif // DIFFTABLE_H
