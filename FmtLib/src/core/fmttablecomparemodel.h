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
    FmtTableCompareModel(QObject *parent = nullptr);
    virtual ~FmtTableCompareModel();

    int rowCount(const QModelIndex &parent = QModelIndex()) const Q_DECL_OVERRIDE;
    int columnCount(const QModelIndex &parent = QModelIndex()) const Q_DECL_OVERRIDE;
    QVariant headerData(int section, Qt::Orientation orientation, int role) const Q_DECL_OVERRIDE;

    void setLists(FmtTable *table, const QString &cppcstruct);

    static void readFmtTable(FmtTable *table, FmtFldElementVector &vec);
    static void readTableStruct(const QString &cppcstruct, FmtFldElementVector &vec);

private:
    static QString extractComments(const QString& structCode, int fieldEndPos);
};

#endif // FMTTABLECOMPAREMODEL_H
