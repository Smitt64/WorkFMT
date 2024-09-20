#ifndef FMTUNDOTABLEADDFIELD_H
#define FMTUNDOTABLEADDFIELD_H

#include <QObject>
#include <QUndoCommand>
#include "fmtlib_global.h"

class FmtTable;
class FmtField;
class FmtUndoTableAddField : public QUndoCommand
{
public:
    FmtUndoTableAddField(FmtTable *table, QUndoCommand *parent = Q_NULLPTR);
    void setNameAndType(const QString &name, const qint16 &type);
    void setInsertBefor(const qint16 &befor);
    FmtField *getField() const;

    virtual void undo();
    virtual void redo();

private:
    FmtTable *pTable;
    FmtField *pField;

    QString m_Name;
    qint16 m_Type;
    qint16 m_Befor, m_Row;
};

#endif // FMTUNDOTABLEADDFIELD_H
