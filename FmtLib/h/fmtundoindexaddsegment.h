#ifndef FMTUNDOINDEXADDSEGMENT_H
#define FMTUNDOINDEXADDSEGMENT_H

#include <QUndoCommand>

class FmtTable;
class FmtSegment;
class FmtIndex;
class FmtUndoIndexAddSegment : public QUndoCommand
{
public:
    FmtUndoIndexAddSegment(FmtTable *table, QUndoCommand *parent = NULL);
    virtual ~FmtUndoIndexAddSegment();
    void setSegmentRow(const quint16 &row, FmtIndex *pIndex);

    FmtSegment *segment();

    virtual void undo();
    virtual void redo();

private:
    FmtTable *pTable;
    FmtSegment *pSegment;
    quint16 m_Segment, m_IndexRow;
    QByteArray m_storedData;
};

#endif // FMTUNDOINDEXADDSEGMENT_H
