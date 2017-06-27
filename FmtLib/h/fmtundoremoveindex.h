#ifndef FMTUNDOREMOVEINDEX_H
#define FMTUNDOREMOVEINDEX_H

#include <QUndoCommand>

class FmtTable;
class FmtUndoRemoveIndex : public QUndoCommand
{
public:
    FmtUndoRemoveIndex(FmtTable *table, QUndoCommand *parent);
    virtual ~FmtUndoRemoveIndex();
    void setIndexRow(const quint16 &row);

    virtual void undo();
    virtual void redo();

private:
    FmtTable *pTable;
    quint16 m_Row;
    quint32 m_PkIndex;
    QByteArray m_storedData;
};

#endif // FMTUNDOREMOVEINDEX_H
