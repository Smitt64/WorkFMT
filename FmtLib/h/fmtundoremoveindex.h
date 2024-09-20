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
    void setIndexRow(const qint16 &row);

    virtual void undo();
    virtual void redo();

private:
    FmtTable *pTable;
    qint16 m_Row;
    qint16 m_PkIndex;
    QByteArray m_storedData;
};

#endif // FMTUNDOREMOVEINDEX_H
