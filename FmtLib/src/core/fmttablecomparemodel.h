#ifndef FMTTABLECOMPAREMODEL_H
#define FMTTABLECOMPAREMODEL_H

#include <QAbstractTableModel>
#include <QVector>
#include <QIcon>

typedef struct
{
    QString name, comment;
    qint16 type;
    qint16 size;
    qint32 offset;
}FmtFldElement;

typedef struct
{
    FmtFldElement mine;
    FmtFldElement theirs;
}FmtTableCompareElement;

typedef QVector<FmtFldElement> FmtFldElementVector;

class FmtTable;
class QXmlStreamReader;
class FmtTableCompareModel : public QAbstractTableModel
{
    Q_OBJECT
public:
    enum
    {
        ColumnId = 0,
        ColumnMineName,
        ColumnMineType,
        ColumnMineSize,

        ColumnTheirsName,
        ColumnTheirsType,
        ColumnTheirsSize,

        ColumnCount
    };
    FmtTableCompareModel(QObject *parent = nullptr);
    virtual ~FmtTableCompareModel();

    int rowCount(const QModelIndex &parent = QModelIndex()) const Q_DECL_OVERRIDE;
    int columnCount(const QModelIndex &parent = QModelIndex()) const Q_DECL_OVERRIDE;
    QVariant headerData(int section, Qt::Orientation orientation, int role) const Q_DECL_OVERRIDE;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const Q_DECL_OVERRIDE;
    Qt::ItemFlags flags(const QModelIndex &index) const Q_DECL_OVERRIDE;

    void setLists(FmtTable *table, const QString &cppcstruct);
    void setLists(FmtTable *table1, FmtTable *table2);
    void setLists(FmtTable *table1, const FmtFldElementVector &table2);
    void setLists(const FmtFldElementVector &table1, const FmtFldElementVector &table2);

    static void readFmtTable(FmtTable *table, FmtFldElementVector &vec);
    static void readTableStruct(const QString &cppcstruct, FmtFldElementVector &vec);

    static bool parseFieldsFromXml(const QString &xmlContent, FmtFldElementVector &fields);

private:
    void makeCompareData(const FmtFldElementVector &mine, const FmtFldElementVector &theirs);
    static QString extractComments(const QString& structCode, int fieldEndPos);
    static bool parseFieldElement(QXmlStreamReader &xml, FmtFldElement &field);

    bool isAddition(const FmtTableCompareElement &row) const;
    bool isDeleton(const FmtTableCompareElement &row) const;

    QIcon m_Warning;
    QVector<FmtTableCompareElement> m_data;
};

#endif // FMTTABLECOMPAREMODEL_H
