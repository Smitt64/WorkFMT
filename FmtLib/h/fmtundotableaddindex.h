#ifndef FMTTABLEADDINDEX_H
#define FMTTABLEADDINDEX_H

#include <QUndoCommand>

class FmtTable;
class FmtIndex;
class FmtUndoTableAddIndex : public QUndoCommand
{
public:
    FmtUndoTableAddIndex(FmtTable *table, QUndoCommand *parent = NULL);
    ~FmtUndoTableAddIndex();
    FmtIndex *getIndex();

    virtual void undo();
    virtual void redo();

private:
    FmtTable *pTable;
    FmtIndex *pIndex;

    qint16 m_Row;
};

#endif // FMTTABLEADDINDEX_H
