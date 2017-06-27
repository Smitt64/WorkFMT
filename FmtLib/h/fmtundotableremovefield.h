#ifndef FMTUNDOTABLEREMOVEFIELD_H
#define FMTUNDOTABLEREMOVEFIELD_H

#include <QUndoCommand>
#include <QByteArray>

class FmtTable;
class FmtUndoTableRemoveField : public QUndoCommand
{
public:
    FmtUndoTableRemoveField(FmtTable *table, QUndoCommand *parent);
    ~FmtUndoTableRemoveField();

    QByteArray *storedDataPtr();
    void setFieldRow(const quint16 &row);

    virtual void undo();
    virtual void redo();
private:
    FmtTable *pTable;
    QByteArray m_StoredData;
    quint16 m_Row;
};

#endif // FMTUNDOTABLEREMOVEFIELD_H
