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
    Q_PROPERTY(quint32 Id READ id NOTIFY idChanged)
    Q_PROPERTY(quint32 TableId READ tableId NOTIFY tableIdChanged)
    Q_PROPERTY(qint32 Size READ size WRITE setSize NOTIFY sizeChanged)
    Q_PROPERTY(qint32 Type READ type WRITE setType NOTIFY typeChanged)
    Q_PROPERTY(qint32 Offset READ offset WRITE setOffset)
    Q_PROPERTY(qint32 Outlen READ outlen WRITE setOutlen)
    Q_PROPERTY(qint32 Decpoint READ decpoint WRITE setDecpoint)
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

    void load(const QSqlRecord &rec);

    quint32 id() const { return m_Id; }
    qint32 size() const { return m_Size; }
    qint32 type() const { return m_Type; }
    quint32 tableId() const;
    quint32 outlen() const;
    quint32 decpoint() const;
    bool isHidden() const;

    QString name() const;
    QString comment() const;
    QString undecorateName() const;

    qint32 offset() const;
    qint32 index() const;
    qint32 typeIndex() const;

    QString getOraName() const;
    QString getOraDecl() const;
    QString getOraTypeName() const;
    QString getCppTypeName(bool Short = false) const;
    QString getCppDecl(bool funcPrm = false) const;

    QString getCommentSql() const;

signals:
    void tableIdChanged(quint32);
    void idChanged(quint32);
    void nameChanged(QString);
    void commentChanged(QString);
    void sizeChanged(quint32);
    void typeChanged(quint32);

public slots:
    void setName(const QString &v);
    void setComment(const QString &v);
    void setSize(const qint32 &v);
    void setOffset(const qint32 &v);
    void setOutlen(const qint32 &v);
    void setType(const qint32 &v);
    void setDecpoint(const qint32 &v);
    void setHidden(const bool &v);
    void setTypeIndex(const qint32 &v);

private:
    explicit FmtField(QObject *parent = 0);
    int FindFirstEmptyID();
    int save();
    void stroreData(QByteArray *data);
    void restoreData(QByteArray *data);
    bool setDataPrivate(const quint16 &fld, const QVariant &value);
    FmtTable *pTable;
    quint32 m_Id;
    qint32 m_Size, m_Type, m_Offset, m_Outlen, m_Decpoint;
    QString m_Name, m_Comment;
    bool m_isHidden;

    bool m_IgnoreUndoStack;
    QUndoStack *pUndoStack;
    QUndoCommand *pLastCommand;

    FmtFildsModel *pFieldsModel;
};

QString FmtFieldPropertyTextById(const quint16 &fld);
#endif // FMTFIELD_H
