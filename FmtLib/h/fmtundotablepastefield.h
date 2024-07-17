#ifndef FMTUNDOTABLEPASTEFIELD_H
#define FMTUNDOTABLEPASTEFIELD_H

#include <QUndoCommand>
#include "fmttable.h"

class FmtUndoTablePasteField : public QUndoCommand
{
public:
    FmtUndoTablePasteField(FmtTable *table, QUndoCommand *parent = Q_NULLPTR);
    virtual ~FmtUndoTablePasteField();

    void setData(const QMap<quint16,QVariant> &data);
    void setInsertBefor(const qint16 &befor);

    FmtField *getField() const;

    virtual void undo();
    virtual void redo();

private:
    FmtTable *pTable;
    FmtField *pField;
    qint16 m_Befor, m_Row;
    QMap<quint16,QVariant> fldDataMap;
};

#endif // FMTUNDOTABLEPASTEFIELD_H
