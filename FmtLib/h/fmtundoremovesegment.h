#ifndef FMTUNDOREMOVESEGMENT_H
#define FMTUNDOREMOVESEGMENT_H

#include <QUndoCommand>

class FmtTable;
class FmtIndex;
class FmtUndoRemoveSegment : public QUndoCommand
{
public:
    FmtUndoRemoveSegment(FmtTable *table, QUndoCommand *parent);

    void setSegmentRow(const quint16 &row, FmtIndex *pIndex);

    virtual void undo();
    virtual void redo();

private:
    FmtTable *pTable;
    QByteArray m_storedData;
    quint16 m_Segment, m_IndexRow;
};

#endif // FMTUNDOREMOVESEGMENT_H
