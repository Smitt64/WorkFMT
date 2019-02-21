#ifndef FMTFIELD_H
#define FMTFIELD_H

#include <QObject>
#include <QtSql>
#include "fmtlib_global.h"

class FmtTable;
class QUndoStack;
class QUndoCommand;
class FmtFildsModel;
class FMTLIBSHARED_EXPORT FmtField : public QObject
{
    Q_OBJECT
    Q_PROPERTY(FmtRecId Id READ id NOTIFY idChanged)
    Q_PROPERTY(quint32 TableId READ tableId NOTIFY tableIdChanged)
    Q_PROPERTY(FmtNumber10 Size READ size WRITE setSize NOTIFY sizeChanged)
    Q_PROPERTY(FmtFldType Type READ type WRITE setType NOTIFY typeChanged)
    Q_PROPERTY(FmtNumber10 Offset READ offset WRITE setOffset)
    Q_PROPERTY(FmtNumber5 Outlen READ outlen WRITE setOutlen)
    Q_PROPERTY(FmtNumber5 Decpoint READ decpoint WRITE setDecpoint)
    Q_PROPERTY(bool Hidden READ isHidden WRITE setHidden)
    Q_PROPERTY(qint32 Index READ index)

    Q_PROPERTY(QString Name READ name WRITE setName NOTIFY nameChanged)
    Q_PROPERTY(QString UndecorateName READ undecorateName)
    Q_PROPERTY(QString Comment READ comment WRITE setComment NOTIFY commentChanged)

    Q_PROPERTY(qint32 TypeIndex READ typeIndex WRITE setTypeIndex)

    friend class FmtTable;
    friend class FmtFildsModel;
    friend class FmtUndoFieldProperty;
    friend class FmtUndoTableRemoveField;
public:
    enum
    {
        fld_Id,
        fld_FmtId,
        fld_Name,
        fld_Type,
        fld_Size,
        fld_Offset,
        fld_Outlen,
        fld_DecPoint,
        fld_Hidden,
        fld_Comment,

        fld_MAXCOUNT
    };
    explicit FmtField(QObject *parent = Q_NULLPTR);
    void load(const QSqlRecord &rec);

    FmtRecId id() const { return m_Id; }
    FmtNumber10 size() const { return m_Size; }
    FmtFldType type() const { return m_Type; }
    FmtRecId tableId() const;
    FmtNumber5 outlen() const;
    FmtNumber5 decpoint() const;
    bool isHidden() const;

    bool isStringType() const;

    QString name() const;
    QString comment() const;
    QString undecorateName() const;

    FmtNumber10 offset() const;
    FmtFldIndex index() const;
    qint32 typeIndex() const;

    void setName(const QString &v);
    void setComment(const QString &v);
    void setSize(const FmtNumber10 &v);
    void setOffset(const FmtNumber10 &v);
    void setOutlen(const FmtNumber5 &v);
    void setType(const FmtFldType &v);
    void setDecpoint(const FmtNumber5 &v);
    void setHidden(const bool &v);
    void setTypeIndex(const FmtFldIndex &v);

    FmtTable *table();

signals:
    void tableIdChanged(quint32);
    void idChanged(quint32);
    void nameChanged(QString);
    void commentChanged(QString);
    void sizeChanged(quint32);
    void typeChanged(quint32);

public slots:    
    QString getOraName() const;
    QString getOraDecl() const;
    QString getOraTypeName() const;
    QString getCppTypeName(bool Short = false) const;
    QString getCppDecl(bool funcPrm = false) const;
    QString getOraDefaultVal() const;
    QString getCommentSql() const;

    bool isNumber() const;
    bool isRealNumber() const;
    bool isString() const;
    bool isAutoInc() const;

private:
    FmtRecId FindFirstEmptyID();
    int save();
    void stroreData(QByteArray *data);
    void restoreData(QByteArray *data);
    bool setDataPrivate(const FmtFldIndex &fld, const QVariant &value);
    FmtTable *pTable;
    FmtRecId m_Id;
    FmtNumber10 m_Size, m_Offset;
    FmtNumber5 m_Outlen, m_Decpoint;
    FmtFldType m_Type;
    QString m_Name, m_Comment;
    bool m_isHidden;

    bool m_IgnoreUndoStack;
    QUndoStack *pUndoStack;
    QUndoCommand *pLastCommand;

    FmtFildsModel *pFieldsModel;
};

QString FmtFieldPropertyTextById(const FmtFldIndex &fld);
#endif // FMTFIELD_H
