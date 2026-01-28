#ifndef GENERATORSPROXYMODEL_H
#define GENERATORSPROXYMODEL_H

#include <QAbstractProxyModel>
#include <QObject>

typedef struct
{
    QString macro;
    QString alias;
    QString highlighter;
}GeneratorsMacroElement;

class FmtGenInterface;
class GeneratorsProxyModel : public QAbstractProxyModel
{
public:
    GeneratorsProxyModel(QObject *parent = nullptr);
    virtual ~GeneratorsProxyModel();

    virtual QModelIndex parent(const QModelIndex &index) const Q_DECL_OVERRIDE;
    virtual int columnCount(const QModelIndex &parent = QModelIndex()) const Q_DECL_OVERRIDE;
    virtual QModelIndex mapFromSource(const QModelIndex &sourceIndex) const Q_DECL_OVERRIDE;
    virtual QModelIndex mapToSource(const QModelIndex &proxyIndex) const Q_DECL_OVERRIDE;
    int rowCount(const QModelIndex& parent = QModelIndex()) const Q_DECL_OVERRIDE;
    QModelIndex index(int row, int column, const QModelIndex& parent = QModelIndex()) const Q_DECL_OVERRIDE;
    virtual Qt::ItemFlags flags(const QModelIndex &index) const Q_DECL_OVERRIDE;
    virtual bool hasChildren(const QModelIndex &parent = QModelIndex()) const Q_DECL_OVERRIDE;
    virtual QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const Q_DECL_OVERRIDE;
    GeneratorsMacroElement getMacroElement(const QModelIndex &index);

private:
    QString ReadValue(const QString &value, const QString &content);
    void ReadFileInfo(GeneratorsMacroElement &elem);
    QList<GeneratorsMacroElement> m_MacroList;
};

void GetGenMacroExecutor(const GeneratorsMacroElement *element, FmtGenInterface **ppObj);

#endif // GENERATORSPROXYMODEL_H
