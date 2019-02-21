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
    void setNameAndType(const QString &name, const FmtFldType &type);
    void setInsertBefor(const FmtFldIndex &befor);
    FmtField *getField() const;

    virtual void undo();
    virtual void redo();

private:
    FmtTable *pTable;
    FmtField *pField;

    QString m_Name;
    FmtFldType m_Type;
    FmtFldIndex m_Befor, m_Row;
};

#endif // FMTUNDOTABLEADDFIELD_H
