#ifndef FMTTABLECOMPAREMODEL_H
#define FMTTABLECOMPAREMODEL_H

#include <QAbstractTableModel>
#include <QVector>

typedef struct
{
    QString name, comment;
    qint16 type;
    qint16 size;
}FmtFldElement;

typedef struct
{
    FmtFldElement mine;
    FmtFldElement theirs;
}FmtTableCompareElement;

typedef QVector<FmtFldElement> FmtFldElementVector;

class FmtTable;
class FmtTableCompareModel : public QAbstractTableModel
{
    Q_OBJECT
public:
    enum
    {
        ColumnMine = 0,
        ColumnTheir
    };
    FmtTableCompareModel(QObject *parent = nullptr);
    virtual ~FmtTableCompareModel();

    int rowCount(const QModelIndex &parent = QModelIndex()) const Q_DECL_OVERRIDE;
    int columnCount(const QModelIndex &parent = QModelIndex()) const Q_DECL_OVERRIDE;
    QVariant headerData(int section, Qt::Orientation orientation, int role) const Q_DECL_OVERRIDE;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const Q_DECL_OVERRIDE;

    void setLists(FmtTable *table, const QString &cppcstruct);

    static void readFmtTable(FmtTable *table, FmtFldElementVector &vec);
    static void readTableStruct(const QString &cppcstruct, FmtFldElementVector &vec);

private:
    void makeCompareData(const FmtFldElementVector &mine, const FmtFldElementVector &theirs);
    static QString extractComments(const QString& structCode, int fieldEndPos);

    QVector<FmtTableCompareElement> m_data;
};

#endif // FMTTABLECOMPAREMODEL_H
