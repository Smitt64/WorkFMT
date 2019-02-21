
#ifndef JSFMTTABLE_H
#define JSFMTTABLE_H

#include <QtScript>
#include <fmttable.h>
#include <fmtcore.h>

void initializeJsFmtTable(QScriptEngine *engine);
class CJsFmtTable : public QObject, public QScriptable
{
    Q_OBJECT

    Q_PROPERTY(quint32 Id READ id)
    Q_PROPERTY(quint32 CacheSize READ cacheSize WRITE setCacheSize)
    Q_PROPERTY(quint32 BlobLen READ blobLen WRITE setBlobLen)
    Q_PROPERTY(quint32 FieldsCount READ fieldsCount)
    Q_PROPERTY(quint32 IndexesCount READ indexesCount)
    Q_PROPERTY(quint32 PkIDx READ pkIDx WRITE setPkIDx)
    Q_PROPERTY(quint16 BlobType READ blobType WRITE setBlobType)
    //Q_PROPERTY(quint32 TableFlags READ tableFlags WRITE setTableFlags)

    Q_PROPERTY(bool isTemporary READ isTemporary WRITE setIsTemporary)
    Q_PROPERTY(bool isRecord READ isRecord WRITE setIsRecord)
    Q_PROPERTY(bool isExists READ isExists)
    Q_PROPERTY(bool isExistsInDb READ isExistsInDb)

    Q_PROPERTY(QString Name READ name WRITE setName)
    Q_PROPERTY(QString DbtName READ dbtName)
    Q_PROPERTY(QString Comment READ comment WRITE setComment)
    Q_PROPERTY(QString Owner READ owner WRITE setOwner)
public:
    enum FmtFieldTypes
    {
        ftINT = fmtt_INT,
        ftLONG = fmtt_LONG,
        ftBIGINT = fmtt_BIGINT,
        ftFLOAT = fmtt_FLOAT,
        ftDOUBLE = fmtt_DOUBLE,
        ftMONEY = fmtt_MONEY,
        ftSTRING = fmtt_STRING,
        ftSNR = fmtt_SNR,
        ftDATE = fmtt_DATE,
        ftTIME = fmtt_TIME,
        ftCHR = fmtt_CHR,
        ftUCHR = fmtt_UCHR,
        ftNUMERIC = fmtt_NUMERIC,
    };
    Q_ENUM(FmtFieldTypes)
    //! \private
    CJsFmtTable(FmtTable *Table, QObject *parent = Q_NULLPTR);
    //! \private
    CJsFmtTable(QSharedPointer<FmtTable> Table, QObject *parent = Q_NULLPTR);

    //! \private
    bool isTemporary() const;
    /// @private
    bool isRecord() const;
    /// @private
    bool isExists() const;
    /// @private
    bool isExistsInDb() const;

    /// @private
    quint32 id() const { return pTable->id(); }
    /// @private
    quint32 cacheSize() const { return pTable->cacheSize(); }
    /// @private
    quint32 blobLen() const { return pTable->blobLen(); }
    /// @private
    quint32 indecesCount() const;
    /// @private
    quint32 pkIDx() const { return pTable->pkIDx(); }
    /// @private
    quint16 blobType() const { return pTable->blobType(); }
    /// @private
    quint32 tableFlags() const { return pTable->tableFlags(); }

    /// @private
    QString name() const;
    /// @private
    QString dbtName() const;
    /// @private
    QString comment() const;
    /// @private
    QString owner() const;

    /// @private
    void setCacheSize(const quint32 &v);
    /// @private
    void setBlobLen(const quint32 &v);
    /// @private
    void setName(const QString &v);
    /// @private
    void setPkIDx(const quint32 &v);
    //void setTableFlags(const quint32 &v);
    /// @private
    void setBlobType(const quint16 &v);
    /// @private
    void setComment(const QString &v);
    /// @private
    void setOwner(const QString &v);
    /// @private
    void setIsTemporary(const bool &v);
    /// @private
    void setIsRecord(const bool &v);

public slots:
    bool load(const QString &name);
    bool load(const quint32 &id);

    FmtField *addField(const QString &name, const qint16 &type);
    FmtField *insertField(const int &befor, const QString &name, const qint16 &type);
    FmtField *field(const qint32 &index);
    qint32 fieldNum(FmtField *fld);
    qint32 fieldsCount() const;

    FmtIndex *addIndex();
    FmtIndex *tableIndex(const qint32 &index);
    quint32 tableIndexNum(FmtIndex *pIndex);
    void removeIndex(const quint32 &index);
    quint32 indexesCount() const;

    QStringList uniqueIndeces() const;

    FmtErrors *checkErrors();

    QSharedPointer<FmtTable> fmtTable();

private:
    /// @private
    QSharedPointer<FmtTable> pTable;
};

#endif // JSFMTTABLE_H
