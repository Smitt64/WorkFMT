#ifndef FMTUNDOTABLEREMOVEFIELD_H
#define FMTUNDOTABLEREMOVEFIELD_H

#include <QUndoCommand>
#include <QByteArray>
#include "fmttable.h"

class FmtTable;
class FmtUndoTableRemoveField : public QUndoCommand
{
public:
    FmtUndoTableRemoveField(FmtTable *table, QUndoCommand *parent);
    ~FmtUndoTableRemoveField();

    QByteArray *storedDataPtr();
    void setFieldRow(const qint16 &row);

    virtual void undo();
    virtual void redo();
private:
    FmtTable *pTable;
    QByteArray m_StoredData;
    qint16 m_Row;
};

#endif // FMTUNDOTABLEREMOVEFIELD_H
