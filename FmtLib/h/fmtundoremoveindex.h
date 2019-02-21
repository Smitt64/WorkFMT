#ifndef FMTUNDOREMOVEINDEX_H
#define FMTUNDOREMOVEINDEX_H

#include <QUndoCommand>
#include "fmttable.h"

class FmtTable;
class FmtUndoRemoveIndex : public QUndoCommand
{
public:
    FmtUndoRemoveIndex(FmtTable *table, QUndoCommand *parent);
    virtual ~FmtUndoRemoveIndex();
    void setIndexRow(const FmtFldIndex &row);

    virtual void undo();
    virtual void redo();

private:
    FmtTable *pTable;
    FmtFldIndex m_Row;
    FmtNumber5 m_PkIndex;
    QByteArray m_storedData;
};

#endif // FMTUNDOREMOVEINDEX_H
