#ifndef SVNSATATUSMODEL_H
#define SVNSATATUSMODEL_H

#include <QAbstractTableModel>
#include <QDateTime>
typedef struct
{
    QString fullpath, path, author, filename;
    QString action, revision, props;
    QDateTime date;
}SvnSatatusElement;

class SvnSatatusModel : public QAbstractTableModel
{
    Q_OBJECT
public:
    enum
    {
        fld_FullFile = 0,
        fld_File,
        fld_FileName,
        fld_Action,

        fld_Count
    };
    explicit SvnSatatusModel(QObject *parent = nullptr);
    virtual ~SvnSatatusModel();

    void setPath(const QString &path);

    virtual QModelIndex parent(const QModelIndex &index) const Q_DECL_OVERRIDE;
    virtual QModelIndex index(int row, int column, const QModelIndex &parent = QModelIndex()) const Q_DECL_OVERRIDE;
    virtual int columnCount(const QModelIndex &parent = QModelIndex()) const Q_DECL_OVERRIDE;
    virtual int rowCount(const QModelIndex &parent = QModelIndex()) const Q_DECL_OVERRIDE;
    virtual QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const Q_DECL_OVERRIDE;

    const SvnSatatusElement &element(const int &row) const;
private:
    QList<SvnSatatusElement> m_Elements;
    QString m_Path;
};

#endif // SVNSATATUSMODEL_H
