#ifndef EXISTSCONDITIONPAGE_H
#define EXISTSCONDITIONPAGE_H

#include <QWizardPage>
#include <QAbstractItemModel>

class FmtTable;
class FmtIndex;
class FmtSegment;
class FmtIndexTreeItem;
class Ui_ExistsConditionPage;
class QButtonGroup;
class FmtIndexTreeModel : public QAbstractItemModel
{
    Q_OBJECT

public:
    explicit FmtIndexTreeModel(FmtTable *table, QObject *parent = nullptr);
    ~FmtIndexTreeModel();

    // QAbstractItemModel interface
    QModelIndex index(int row, int column, const QModelIndex &parent = QModelIndex()) const override;
    QModelIndex parent(const QModelIndex &child) const override;
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    int columnCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    bool setData(const QModelIndex &index, const QVariant &value, int role) override;
    Qt::ItemFlags flags(const QModelIndex &index) const override;

    // Custom methods
    FmtIndex* getIndex(const QModelIndex &index) const;
    bool isIndex(const QModelIndex &index) const;
    bool isSegment(const QModelIndex &index) const;

    // Selection methods
    FmtIndex* getSelectedIndex() const;
    QModelIndex getSelectedIndexModelIndex() const;
    void setSelectedIndex(const QModelIndex &index);
    void clearSelection();

signals:
    void selectionChanged(FmtIndex* selectedIndex);

private:
    void setupModelData();
    QModelIndex findIndexForIndex(FmtIndex *index) const;
    FmtIndexTreeItem *getItem(const QModelIndex &index) const;

private:
    FmtIndexTreeItem *m_rootItem = nullptr;
    FmtTable *m_table = nullptr;
    FmtIndex *m_selectedIndex = nullptr;
};

// -------------------------------------------------------------------------------------

class ExistsConditionPage : public QWizardPage
{
    Q_OBJECT

public:
    explicit ExistsConditionPage(QWidget *parent = nullptr);
    ~ExistsConditionPage();

    void initializePage() override;
    FmtIndex* getSelectedIndex() const;
    QString getCustomCondition() const;
    bool useCustomCondition() const;

private slots:
    void onTreeViewActivated(const QModelIndex &index);
    void onSelectionChanged(FmtIndex* selectedIndex);
    void onCustomConditionChanged();

private:
    Ui_ExistsConditionPage *ui;
    FmtIndexTreeModel *m_pModel;
    QButtonGroup *m_pGroup;
};

#endif // EXISTSCONDITIONPAGE_H
