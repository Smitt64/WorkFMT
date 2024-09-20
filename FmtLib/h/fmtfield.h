#ifndef FMTFIELD_H
#define FMTFIELD_H

#include <QObject>
#include <QtSql>
#include "fmtlib_global.h"
#include "LockVar.hpp"

class FmtTable;
class QUndoStack;
class QUndoCommand;
class FmtFildsModel;
class FMTLIBSHARED_EXPORT FmtField : public QObject
{
    Q_OBJECT
    Q_PROPERTY(quint64 Id READ id NOTIFY idChanged)
    Q_PROPERTY(quint32 TableId READ tableId NOTIFY tableIdChanged)
    Q_PROPERTY(qint32 Size READ size WRITE setSize NOTIFY sizeChanged)
    Q_PROPERTY(qint16 Type READ type WRITE setType NOTIFY typeChanged)
    Q_PROPERTY(qint32 Offset READ offset WRITE setOffset)
    Q_PROPERTY(qint16 Outlen READ outlen WRITE setOutlen)
    Q_PROPERTY(qint16 Decpoint READ decpoint WRITE setDecpoint)
    Q_PROPERTY(bool Hidden READ isHidden WRITE setHidden)
    Q_PROPERTY(qint32 Index READ index)
    Q_PROPERTY(bool isNumber READ isNumber CONSTANT)
    Q_PROPERTY(bool isRealNumber READ isRealNumber CONSTANT)
    Q_PROPERTY(bool isString READ isString CONSTANT)
    Q_PROPERTY(bool isAutoInc READ isAutoInc CONSTANT)

    Q_PROPERTY(QString Name READ name WRITE setName NOTIFY nameChanged)
    Q_PROPERTY(QString UndecorateName READ undecorateName)
    Q_PROPERTY(QString Comment READ comment WRITE setComment NOTIFY commentChanged)

    Q_PROPERTY(qint32 TypeIndex READ typeIndex WRITE setTypeIndex)

    friend class FmtTable;
    friend class FmtFildsModel;
    friend class FmtUndoFieldProperty;
    friend class FmtUndoTableRemoveField;
    friend QDataStream &operator <<(QDataStream &stream, FmtField *fld);
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

    quint64 id() const { return m_Id; }
    qint32 size() const { return m_Size; }
    qint16 type() const { return m_Type; }
    quint64 tableId() const;
    qint16 outlen() const;
    qint16 decpoint() const;
    bool isHidden() const;

    bool isStringType() const;

    QString name() const;
    QString comment() const;
    QString undecorateName() const;

    qint32 offset() const;
    qint16 index() const;
    qint32 typeIndex() const;

    void setName(const QString &v);
    void setComment(const QString &v);
    void setSize(const qint32 &v);
    void setOffset(const qint32 &v);
    void setOutlen(const qint16 &v);
    void setType(const qint16 &v);
    void setDecpoint(const qint16 &v);
    void setHidden(const bool &v);
    void setTypeIndex(const qint16 &v);

    FmtTable *table();

    static QString getMimeType();

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
    QString getRsdType() const;
    QString getRsdConstant() const;

    bool isNumber() const;
    bool isRealNumber() const;
    bool isString() const;
    bool isAutoInc() const;

private:
    quint64 FindFirstEmptyID();
    int save();
    void stroreData(QByteArray *data);
    void stroreData(QDataStream &stream);
    void restoreData(QByteArray *data);
    bool setDataPrivate(const qint16 &fld, const QVariant &value);
    FmtTable *pTable;
    quint64 m_Id;
    qint32 m_Size, m_Offset;
    qint16 m_Outlen, m_Decpoint;
    qint16 m_Type;
    QString m_Name, m_Comment;
    bool m_isHidden;

    LockedVar<bool> m_IgnoreUndoStack;
    QUndoStack *pUndoStack;
    QUndoCommand *pLastCommand;

    FmtFildsModel *pFieldsModel;
};

QDataStream &operator <<(QDataStream &stream, FmtField *fld);
QString FmtFieldPropertyTextById(const qint16 &fld);
#endif // FMTFIELD_H
