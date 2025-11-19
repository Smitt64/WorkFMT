#ifndef REGPARMPROXYMODEL_H
#define REGPARMPROXYMODEL_H

#include <QSortFilterProxyModel>
#include <QSet>

class RegParmProxyModel : public QSortFilterProxyModel
{
    Q_OBJECT

public:
    explicit RegParmProxyModel(QObject *parent = nullptr);

    // QSortFilterProxyModel interface
    bool filterAcceptsRow(int source_row, const QModelIndex &source_parent) const override;
    bool filterAcceptsColumn(int source_column, const QModelIndex &source_parent) const override;

    // Custom methods
    void setFilterString(const QString &filter);
    void setShowOnlyMatches(bool showOnly);

protected:
    bool hasAcceptedChildren(int source_row, const QModelIndex &source_parent) const;

private:
    bool nodeMatchesFilter(int source_row, const QModelIndex &source_parent) const;
    bool acceptsAnyChild(int source_row, const QModelIndex &source_parent) const;
    bool hasMatchingParent(int source_row, const QModelIndex &source_parent) const;

    QString m_filterString;
    bool m_showOnlyMatches = false;
    QSet<int> m_visibleColumns;
};

#endif // REGPARMPROXYMODEL_H
