#ifndef EXISTSCONDITIONPAGE_H
#define EXISTSCONDITIONPAGE_H

#include <QWizardPage>
#include <QAbstractItemModel>

namespace Ui {
class ExistsConditionPage;
}

class FmtIndexTreeItem;
class FmtTable;
class FmtIndex;
class FmtIndexTreeModel : public QAbstractItemModel
{
    Q_OBJECT

public:
    explicit FmtIndexTreeModel(FmtTable *table, QObject *parent = nullptr);
    ~FmtIndexTreeModel();

    // QAbstractItemModel interface
    QModelIndex index(int row, int column, const QModelIndex &parent) const Q_DECL_OVERRIDE;
    QModelIndex parent(const QModelIndex &child) const Q_DECL_OVERRIDE;
    int rowCount(const QModelIndex &parent) const Q_DECL_OVERRIDE;
    int columnCount(const QModelIndex &parent) const Q_DECL_OVERRIDE;
    QVariant data(const QModelIndex &index, int role) const Q_DECL_OVERRIDE;
    Qt::ItemFlags flags(const QModelIndex &index) const Q_DECL_OVERRIDE;

    // Получение выбранного индекса
    FmtIndex* getIndex(const QModelIndex &index) const;
    bool isIndex(const QModelIndex &index) const;
    bool isSegment(const QModelIndex &index) const;

private:
    void setupModelData();
    FmtIndexTreeItem *getItem(const QModelIndex &index) const;

    FmtTable *m_table;
    FmtIndexTreeItem *m_rootItem;
};

class QButtonGroup;
class ExistsConditionPage : public QWizardPage
{
    Q_OBJECT

public:
    explicit ExistsConditionPage(QWidget *parent = nullptr);
    ~ExistsConditionPage();

    virtual void initializePage() Q_DECL_OVERRIDE;

private:
    Ui::ExistsConditionPage *ui;

    QButtonGroup *m_pGroup;
    FmtIndexTreeModel *m_pModel;
};

#endif // EXISTSCONDITIONPAGE_H
