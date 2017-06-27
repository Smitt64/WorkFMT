#ifndef FMTUNDOINDEXSEGMENTPROPERTY_H
#define FMTUNDOINDEXSEGMENTPROPERTY_H

#include <QUndoCommand>
#include <QVariant>

class FmtTable;
class FmtUndoIndexSegmentProperty : public QUndoCommand
{
public:
    FmtUndoIndexSegmentProperty(FmtTable *table, QUndoCommand *parent = NULL);
    virtual ~FmtUndoIndexSegmentProperty();

    void setValueToUndo(const QVariant &value);
    void setValueToRedo(const QVariant &value);
    void setProperty(const quint16 &index, const quint16 &segment, const quint16 &property);

    virtual void undo();
    virtual void redo();

private:
    FmtTable *pTable;

    QString m_PropertyDisplayName;
    quint16 m_Property, m_Index, m_Segment;
    QVariant m_UndoValue, m_RedoValue;
};

#endif // FMTUNDOINDEXSEGMENTPROPERTY_H
