#ifndef FMTUNDOINDEXSEGMENTPROPERTY_H
#define FMTUNDOINDEXSEGMENTPROPERTY_H

#include <QUndoCommand>
#include <QVariant>
#include "fmtcore.h"

class FmtTable;
class FmtUndoIndexSegmentProperty : public QUndoCommand
{
public:
    FmtUndoIndexSegmentProperty(FmtTable *table, QUndoCommand *parent = Q_NULLPTR);
    virtual ~FmtUndoIndexSegmentProperty();

    void setValueToUndo(const QVariant &value);
    void setValueToRedo(const QVariant &value);
    void setProperty(const FmtFldIndex &index, const FmtFldIndex &segment, const quint16 &property);

    virtual void undo();
    virtual void redo();

private:
    FmtTable *pTable;

    QString m_PropertyDisplayName;
    FmtFldIndex m_Index, m_Segment;
    quint16 m_Property;
    QVariant m_UndoValue, m_RedoValue;
};

#endif // FMTUNDOINDEXSEGMENTPROPERTY_H
