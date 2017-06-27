#ifndef FMTUNDOTABLEADDFIELD_H
#define FMTUNDOTABLEADDFIELD_H

#include <QObject>
#include <QUndoCommand>

class FmtTable;
class FmtField;
class FmtUndoTableAddField : public QUndoCommand
{
public:
    FmtUndoTableAddField(FmtTable *table, QUndoCommand *parent = NULL);
    void setNameAndType(const QString &name, const qint16 &type);
    void setInsertBefor(const qint32 &befor);
    FmtField *getField() const;

    virtual void undo();
    virtual void redo();

private:
    FmtTable *pTable;
    FmtField *pField;

    QString m_Name;
    qint16 m_Type;
    qint32 m_Befor, m_Row;
};

#endif // FMTUNDOTABLEADDFIELD_H
