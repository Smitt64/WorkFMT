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
class QAction;
class FMTLIBSHARED_EXPORT FmtTable : public QAbstractItemModel
{
    Q_OBJECT
    Q_PROPERTY(FmtRecId Id READ id NOTIFY idChanged)
    Q_PROPERTY(FmtNumber10 CacheSize READ cacheSize WRITE setCacheSize NOTIFY cacheSizeChanged)
    Q_PROPERTY(FmtNumber10 BlobLen READ blobLen WRITE setBlobLen NOTIFY blobLenChanged)
    Q_PROPERTY(quint32 FieldsCount READ fieldsCount)
    Q_PROPERTY(FmtNumber5 PkIDx READ pkIDx WRITE setPkIDx)
    Q_PROPERTY(FmtNumber5 BlobType READ blobType WRITE setBlobType)
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
    explicit FmtTable(ConnectionInfo *connection, QObject *parent = Q_NULLPTR);
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
    FmtRecId id() const { return m_Id; }
    /// @private
    FmtNumber10 cacheSize() const { return m_CacheSize; }
    /// @private
    FmtNumber10 blobLen() const { return m_BlobLen; }
    /// @private
    FmtNumber5 indecesCount() const;
    /// @private
    FmtNumber5 pkIDx() const { return m_PkIDx; }
    /// @private
    FmtNumber5 blobType() const { return m_BlobType; }
    /// @private
    FmtNumber10 tableFlags() const { return m_Flags; }

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

    void setCacheSize(const FmtNumber10 &v);
    void setBlobLen(const FmtNumber10 &v);
    void setName(const QString &v);
    void setPkIDx(const FmtNumber5 &v);
    void setTableFlags(const FmtNumber10 &v);
    void setBlobType(const FmtNumber5 &v);
    void setComment(const QString &v);
    void setOwner(const QString &v);
    void setIsTemporary(const bool &v);
    void setIsRecord(const bool &v);

    void copyTo(QSharedPointer<FmtTable> cTable);

signals:
    void idChanged(FmtRecId);
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
    bool load(const FmtRecId &id);
    bool load(const QString &name);

    FmtField *addField(const QString &name, const FmtFldType &type);
    FmtField *addField(const QMap<quint16,QVariant> &data);
    FmtField *insertField(const FmtFldIndex &befor, const QString &name, const FmtFldType &type);
    FmtField *field(const FmtNumber5 &index);
    FmtFldIndex fieldNum(FmtField *fld);
    FmtFldIndex fieldsCount() const;
    bool isFieldAutoInc(FmtField *fld) const;
    bool isFieldAutoInc(const FmtFldIndex &index) const;

    FmtIndex *addIndex();
    FmtIndex *tableIndex(const FmtNumber5 &index);
    FmtFldIndex tableIndexNum(FmtIndex *pIndex);
    void removeIndex(const FmtFldIndex &index);

    QString generateCreateTableSql();
    QString getCommentSql();

    bool hasUniqueIndexes() const;
    bool hasNonUniqueIndexes() const;

    void removeField(const FmtFldIndex &row);
    void rebuildOffsets(QUndoCommand *pLastCommand = Q_NULLPTR);
    void clear();

    int save();
    int safeSave(const bool &IgnoreWarnings);
    int createDbTable(QString *err = Q_NULLPTR);
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
    FmtRecId FindFirstEmptyID();
    /// @private
    int deleteDbTable();
    /// @private
    qint16 calcMaxOraLenght(qint16 *maxfieldname);
    /// @private
    FmtField *FindField(const FmtRecId &Id);
    /// @private
    bool setDataPrivate(const FmtFldType &fld, const QVariant &value);
    /// @private
    FmtField *addFieldPrivate(const QString &name, const FmtFldType &type);
    /// @private
    FmtField *addFieldPrivate(const QMap<quint16,QVariant> &data);
    /// @private
    FmtField *insertFieldPrivate(const FmtFldIndex &befor, const QString &name, const FmtFldType &type);
    /// @private
    FmtField *insertFieldPrivate(const FmtFldIndex &befor, const QMap<quint16,QVariant> &data);
    /// @private
    void removeInsertedFieldPrivate(const FmtFldIndex &row);
    /// @private
    void removeFieldPrivate(const FmtFldIndex &row);
    /// @private
    FmtIndex *addIndexPrivate(const FmtFldIndex &row = -1);
    /// @private
    void removeIndexPrivate(const FmtFldIndex &index, bool AutoDelete = true);
    /// @private
    bool setIgnoreUndoStack(const bool &v);

    FmtRecId m_Id;
    FmtNumber10 m_CacheSize, m_BlobLen, m_Flags;
    FmtNumber5 m_PkIDx, m_BlobType;
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

QString FmtTablePropertyByFieldId(const FmtFldIndex &fld);
QString FmtTablePropertyTextByFieldId(const FmtFldIndex &fld);
QString FmtTableMakeIndexName(FmtTable *pTable, const FmtFldIndex &indexNum);
typedef QSharedPointer<FmtTable> FmtSharedTablePtr;

#endif // FMTTABLE_H
