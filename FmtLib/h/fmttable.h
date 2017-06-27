#ifndef FMTTABLE_H
#define FMTTABLE_H

#include <QObject>
#include <QtSql>
#include <QSharedPointer>
#include "fmtlib_global.h"

class FmtFildsModel;
class FmtField;
class FmtIndecesModel;
class FmtIndex;
class FmtErrors;
class ConnectionInfo;
class QUndoStack;
class QUndoCommand;
class FMTLIBSHARED_EXPORT FmtTable : public QAbstractItemModel
{
    Q_OBJECT
    Q_PROPERTY(quint32 Id READ id NOTIFY idChanged)
    Q_PROPERTY(quint32 CacheSize READ cacheSize WRITE setCacheSize NOTIFY cacheSizeChanged)
    Q_PROPERTY(quint32 BlobLen READ blobLen WRITE setBlobLen NOTIFY blobLenChanged)
    Q_PROPERTY(quint32 FieldsCount READ fieldsCount)
    Q_PROPERTY(quint32 PkIDx READ pkIDx WRITE setPkIDx)
    Q_PROPERTY(quint16 BlobType READ blobType WRITE setBlobType)
    Q_PROPERTY(quint32 TableFlags READ tableFlags WRITE setTableFlags)

    Q_PROPERTY(bool isTemporary READ isTemporary WRITE setIsTemporary NOTIFY isTemporaryChanged)
    Q_PROPERTY(bool isRecord READ isRecord WRITE setIsRecord NOTIFY isRecordChanged)
    Q_PROPERTY(bool isExists READ isExists)
    Q_PROPERTY(bool isExistsInDb READ isExistsInDb)

    Q_PROPERTY(QString Name READ name WRITE setName NOTIFY nameChanged)
    Q_PROPERTY(QString DbtName READ dbtName)
    Q_PROPERTY(QString Comment READ comment WRITE setComment NOTIFY commentChanged)
    Q_PROPERTY(QString Owner READ owner WRITE setOwner)

    friend class FmtFildsModel;
    friend class FmtIndecesModel;
    friend class FmtIndex;
    friend class FmtField;
    friend class FmtSegment;
    friend class FmtUndoTableProperty;
    friend class FmtUndoTableAddField;
    friend class FmtUndoTableRemoveField;
    friend class FmtUndoTableAddIndex;
    friend class FmtUndoRemoveIndex;
    friend class FmtUndoRemoveSegment;
    friend class FmtUndoIndexAddSegment;
    friend class FmtUndoIndexProperty;
    friend class FmtUndoIndexSegmentProperty;
public:
    /// @private
    enum
    {
        fld_Id,
        fld_Name,
        fld_CacheSize,
        fld_Owner,
        fld_Comment,
        fld_PkIndx,
        fld_BlobType,
        fld_BlobLen,
        fld_Flags,

        fld_isTemporary,
        fld_isRecord,
    };
    explicit FmtTable(QObject *parent = 0);
    explicit FmtTable(ConnectionInfo *connection, QObject *parent = 0);
    ~FmtTable();

    /// @private
    bool isTemporary() const;
    /// @private
    bool isRecord() const;
    /// @private
    bool isExists() const;
    /// @private
    bool isExistsInDb() const;

    /// @private
    quint32 id() const { return m_Id; }
    /// @private
    quint32 cacheSize() const { return m_CacheSize; }
    /// @private
    quint32 blobLen() const { return m_BlobLen; }
    /// @private
    quint32 fieldsCount() const;
    /// @private
    quint32 indecesCount() const;
    /// @private
    quint32 pkIDx() const { return m_PkIDx; }
    /// @private
    quint16 blobType() const { return m_BlobType; }
    /// @private
    quint32 tableFlags() const { return m_Flags; }

    /// @private
    QString name() const;
    /// @private
    QString dbtName() const;
    /// @private
    QString comment() const;
    /// @private
    QString owner() const;

    /// @private
    virtual int columnCount(const QModelIndex &parent = QModelIndex()) const;
    /// @private
    virtual QVariant data (const QModelIndex &index, int role = Qt::DisplayRole) const;
    /// @private
    virtual QModelIndex index (int row, int column, const QModelIndex &parent = QModelIndex()) const;
    /// @private
    virtual QModelIndex parent (const QModelIndex &index) const;
    /// @private
    virtual int rowCount (const QModelIndex &parent = QModelIndex()) const;
    /// @private
    virtual Qt::ItemFlags flags(const QModelIndex &index) const;
    /// @private
    virtual bool setData(const QModelIndex &index, const QVariant &value, int role = Qt::EditRole);

    /// @private
    QUndoStack *undoStack();

    QAction *undoAction();
    QAction *redoAction();

    void setCacheSize(const quint32 &v);
    void setBlobLen(const quint32 &v);
    void setName(const QString &v);
    void setPkIDx(const quint32 &v);
    void setTableFlags(const quint32 &v);
    void setBlobType(const quint16 &v);
    void setComment(const QString &v);
    void setOwner(const QString &v);
    void setIsTemporary(const bool &v);
    void setIsRecord(const bool &v);

    void copyTo(QSharedPointer<FmtTable> cTable);

signals:
    void idChanged(quint32);
    void cacheSizeChanged(quint32);
    void blobLenChanged(quint32);
    void nameChanged(QString);
    void commentChanged(QString);
    void isTemporaryChanged(bool);
    void isRecordChanged(bool);

    void indexAdded(FmtIndex *indx);
    void indexChanged(FmtIndex *indx);
    void indexRemoved(const quint32 &pos);

    void fieldRemoved(const int &row);
    void fieldAdded(FmtField *fld);

public slots:
    ConnectionInfo *connection();
    bool load(const quint32 &id);
    bool load(const QString &name);

    FmtField *addField(const QString &name, const qint16 &type);
    FmtField *insertField(const int &befor, const QString &name, const qint16 &type);
    FmtField *field(const qint32 &index);
    qint32 fieldNum(FmtField *fld);

    FmtIndex *addIndex();
    FmtIndex *tableIndex(const qint32 &index);
    quint32 tableIndexNum(FmtIndex *pIndex);
    void removeIndex(const quint32 &index);

    QString generateCreateTableSql();
    QString getCommentSql();

    bool hasUniqueIndexes() const;
    bool hasNonUniqueIndexes() const;

    void removeField(const int &row);
    void rebuildOffsets(QUndoCommand *pLastCommand = NULL);
    void clear();

    int save();
    int safeSave(const bool &IgnoreWarnings);
    int createDbTable();
    int dbInit(const QString &log = QString());

    bool checkErrors(FmtErrors *e);
    FmtErrors *lastErrors();

    FmtFildsModel *fieldsModel() { return pFieldsModel; }
    FmtIndecesModel *indecesModel() { return pIndecesModel; }
    QStringList uniqueIndeces() const;

private slots:
    /// @private
    void onIndexChanged();

private:
    /// @private
    void FillIndex();
    /// @private
    void init();
    /// @private
    int RemoveTrn();
    /// @private
    int SaveTrn();
    /// @private
    int FindFirstEmptyID();
    /// @private
    int deleteDbTable();
    /// @private
    qint16 calcMaxOraLenght(qint16 *maxfieldname);
    /// @private
    FmtField *FindField(const quint32 &Id);
    /// @private
    bool setDataPrivate(const quint16 &fld, const QVariant &value);
    /// @private
    FmtField *addFieldPrivate(const QString &name, const qint16 &type);
    /// @private
    FmtField *insertFieldPrivate(const int &befor, const QString &name, const qint16 &type);
    /// @private
    void removeInsertedFieldPrivate(const int &row);
    /// @private
    void removeFieldPrivate(const int &row);
    /// @private
    FmtIndex *addIndexPrivate(const int &row = -1);
    /// @private
    void removeIndexPrivate(const quint32 &index, bool AutoDelete = true);
    /// @private
    bool setIgnoreUndoStack(const bool &v);

    quint32 m_Id, m_CacheSize, m_BlobLen, m_Flags;
    qint16 m_PkIDx, m_BlobType;
    QString m_Name, m_Owner, m_Comment;

    QSqlRecord m_Record;
    QSqlDatabase db;

    QList<FmtField*> m_pFields;
    QList<FmtIndex*> m_pIndeces;

    FmtFildsModel *pFieldsModel;
    FmtIndecesModel *pIndecesModel;

    ConnectionInfo *pConnection;

    QUndoStack *pUndoStack;
    QAction *pUndoAction, *pRedoAction;

    bool m_IgnoreUndoStack;

    FmtErrors *m_pErrors;
};

QString FmtTablePropertyByFieldId(const quint16 &fld);
QString FmtTablePropertyTextByFieldId(const quint16 &fld);
QString FmtTableMakeIndexName(FmtTable *pTable, const quint16 &indexNum);
typedef QSharedPointer<FmtTable> FmtSharedTablePtr;

#endif // FMTTABLE_H
