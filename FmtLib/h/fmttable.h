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
class ErrorsModel;
class ConnectionInfo;
class QUndoStack;
class QUndoCommand;
class QAction;
class FMTLIBSHARED_EXPORT FmtTable : public QAbstractItemModel
{
    Q_OBJECT
    Q_PROPERTY(qint64 Id READ id NOTIFY idChanged)
    Q_PROPERTY(qint32 CacheSize READ cacheSize WRITE setCacheSize NOTIFY cacheSizeChanged)
    Q_PROPERTY(qint32 BlobLen READ blobLen WRITE setBlobLen NOTIFY blobLenChanged)
    Q_PROPERTY(quint32 FieldsCount READ fieldsCount)
    Q_PROPERTY(quint32 IndecesCount READ indecesCount)
    Q_PROPERTY(qint16 PkIDx READ pkIDx WRITE setPkIDx)
    Q_PROPERTY(qint16 BlobType READ blobType WRITE setBlobType)
    Q_PROPERTY(quint32 TableFlags READ tableFlags WRITE setTableFlags)

    Q_PROPERTY(QObject *connection READ connectionObj CONSTANT)

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
    friend class FmtUndoTablePasteField;
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
    explicit FmtTable(QObject *parent = Q_NULLPTR);
    Q_INVOKABLE FmtTable(ConnectionInfo *connection, QObject *parent = Q_NULLPTR);
    ~FmtTable();

    /// @private
    bool isTemporary() const;
    /// @private
    bool isRecord() const;
    /// @private
    bool isExists() const;
    /// @private
    bool isExistsInDb() const;

    quint64 id() const { return m_Id; }
    /// @private
    qint32 cacheSize() const { return m_CacheSize; }
    /// @private
    qint32 blobLen() const { return m_BlobLen; }
    /// @private
    qint16 indecesCount() const;
    /// @private
    qint16 pkIDx() const { return m_PkIDx; }
    /// @private
    qint16 blobType() const { return m_BlobType; }
    /// @private
    qint32 tableFlags() const { return m_Flags; }

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

    /// @private
    int removeFmtTable();

    QAction *undoAction();
    QAction *redoAction();

    void setCacheSize(const qint32 &v);
    void setBlobLen(const qint32 &v);
    void setName(const QString &v);
    void setPkIDx(const qint16 &v);
    void setTableFlags(const qint32 &v);
    void setBlobType(const qint16 &v);
    void setComment(const QString &v);
    void setOwner(const QString &v);
    void setIsTemporary(const bool &v);
    void setIsRecord(const bool &v);

    void copyTo(QSharedPointer<FmtTable> cTable);
    void copyToAsTmp(QSharedPointer<FmtTable> cTable);

signals:
    void idChanged(quint64);
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

public:
    ConnectionInfo *connection();
    QObject *connectionObj();
    Q_INVOKABLE bool load(const quint64 &id);
    Q_INVOKABLE bool load(const QString &name);
    Q_INVOKABLE bool loadFromXml(const QString &filename, const QString &tableName = QString());

    Q_INVOKABLE FmtField *addField(const QString &name, const qint16 &type);
    Q_INVOKABLE FmtField *addField(const QMap<quint16,QVariant> &data);

    Q_INVOKABLE FmtField *insertField(const qint16 &befor, const QString &name, const qint16 &type);
    Q_INVOKABLE FmtField *field(const qint16 &index);
    Q_INVOKABLE qint16 fieldNum(FmtField *fld);
    qint32 fieldsCount() const;
    Q_INVOKABLE bool isFieldAutoInc(FmtField *fld) const;
    Q_INVOKABLE bool isFieldAutoInc(const qint16 &index) const;

    Q_INVOKABLE FmtIndex *addIndex();
    Q_INVOKABLE FmtIndex *tableIndex(const qint16 &index);
    Q_INVOKABLE qint16 tableIndexNum(const FmtIndex *pIndex);
    Q_INVOKABLE void removeIndex(const qint16 &index);

    QString generateCreateTableSql();
    QString getCommentSql();

    Q_INVOKABLE bool hasUniqueIndexes() const;
    Q_INVOKABLE bool hasNonUniqueIndexes() const;

    Q_INVOKABLE void removeField(const qint16 &row);
    Q_INVOKABLE void rebuildOffsets(QUndoCommand *pLastCommand = Q_NULLPTR);
    Q_INVOKABLE void clear();

    Q_INVOKABLE int save();
    int safeSave(const bool &IgnoreWarnings);
    qint16 createDbTable(QString *err = Q_NULLPTR);
    int dbInit(const QString &log = QString());

    Q_INVOKABLE bool checkErrors(ErrorsModel *e);
    Q_INVOKABLE ErrorsModel *lastErrors();

    FmtFildsModel *fieldsModel() { return pFieldsModel; }
    FmtIndecesModel *indecesModel() { return pIndecesModel; }
    QStringList uniqueIndeces() const;

    const QList<FmtField*> &getFieldsList() const;

private slots:
    /// @private
    void onIndexChanged();

private:
    /// @private
    void FillIndex();
    /// @private
    void init();
    int RemoveByID(const qint32 &id);
    /// @private
    int RemoveTrn();
    /// @private
    int SaveTrn();
    /// @private
    quint64 FindFirstEmptyID();
    /// @private
    int deleteDbTable();
    /// @private
    qint16 calcMaxOraLenght(qint16 *maxfieldname);
    /// @private
    FmtField *FindField(const quint64 &Id);
    /// @private
    bool setDataPrivate(const qint16 &fld, const QVariant &value);
    /// @private
    FmtField *addFieldPrivate(const QString &name, const qint16 &type);
    /// @private
    FmtField *addFieldPrivate(const QMap<quint16,QVariant> &data);
    /// @private
    FmtField *insertFieldPrivate(const qint16 &befor, const QString &name, const qint16 &type);
    /// @private
    FmtField *insertFieldPrivate(const qint16 &befor, const QMap<quint16,QVariant> &data);
    /// @private
    void removeInsertedFieldPrivate(const qint16 &row);
    /// @private
    void removeFieldPrivate(const qint16 &row);
    /// @private
    FmtIndex *addIndexPrivate(const qint16 &row = -1);
    /// @private
    void removeIndexPrivate(const qint16 &index, bool AutoDelete = true);
    /// @private
    bool setIgnoreUndoStack(const bool &v);

    quint64 m_Id;
    qint32 m_CacheSize, m_BlobLen, m_Flags;
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

    ErrorsModel *m_pErrors;
};

QString FmtTablePropertyByFieldId(const qint16 &fld);
QString FmtTablePropertyTextByFieldId(const qint16 &fld);
QString FmtTableMakeIndexName(FmtTable *pTable, const qint16 &indexNum);
QString FmtTableFindFirstEmptyIDSql(const QString &table, const QString &fld);
typedef QSharedPointer<FmtTable> FmtSharedTablePtr;

#endif // FMTTABLE_H
