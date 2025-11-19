#include "regparmproxymodel.h"
#include "regparmitem.h"
#include "regparmmodel.h"
#include <QModelIndex>
#include <QDebug>

RegParmProxyModel::RegParmProxyModel(QObject *parent)
    : QSortFilterProxyModel(parent)
{
    m_visibleColumns = {
        RegParmItem::T_NAME,
        RegParmItem::T_TYPE,
        RegParmItem::T_GLOBAL,
        RegParmItem::T_SECURITY,
        RegParmItem::T_ISBRANCH,
        RegParmItem::T_DESCRIPTION
    };

    setFilterCaseSensitivity(Qt::CaseInsensitive);
    setRecursiveFilteringEnabled(true);
}

bool RegParmProxyModel::filterAcceptsRow(int source_row, const QModelIndex &source_parent) const
{
    // Если фильтр пустой, показываем все
    if (m_filterString.isEmpty())
        return true;

    // Проверяем текущий узел
    QModelIndex index = sourceModel()->index(source_row, RegParmItem::T_NAME, source_parent);
    if (index.isValid()) {
        QString name = sourceModel()->data(index).toString();
        if (name.contains(m_filterString, Qt::CaseInsensitive)) {
            // Если узел совпал с фильтром, показываем ВСЕ его дочерние узлы
            return true;
        }
    }

    // Проверяем, есть ли у этого узла родитель, который совпал с фильтром
    if (hasMatchingParent(source_row, source_parent)) {
        return true;
    }

    // Проверяем, есть ли у этого узла дети, которые подходят под фильтр
    if (hasAcceptedChildren(source_row, source_parent)) {
        return true;
    }

    return false;
}

bool RegParmProxyModel::filterAcceptsColumn(int source_column, const QModelIndex &source_parent) const
{
    Q_UNUSED(source_parent)
    return m_visibleColumns.contains(source_column);
}

bool RegParmProxyModel::hasAcceptedChildren(int source_row, const QModelIndex &source_parent) const
{
    QModelIndex parentIndex = sourceModel()->index(source_row, 0, source_parent);
    if (!parentIndex.isValid())
        return false;

    // Принудительно загружаем детей для этого узла
    RegParmModel *sourceModel = qobject_cast<RegParmModel*>(this->sourceModel());
    if (sourceModel) {
        sourceModel->blockSignals(true);
        sourceModel->loadChildren(parentIndex);
        sourceModel->blockSignals(false);
    }

    // Теперь проверяем детей
    const int childCount = sourceModel->rowCount(parentIndex);
    for (int i = 0; i < childCount; ++i) {
        if (nodeMatchesFilter(i, parentIndex))
            return true;
        if (hasAcceptedChildren(i, parentIndex))
            return true;
    }

    return false;
}

bool RegParmProxyModel::hasMatchingParent(int source_row, const QModelIndex &source_parent) const
{
    // Проверяем всех родителей вверх по иерархии
    QModelIndex currentIndex = sourceModel()->index(source_row, 0, source_parent);

    while (currentIndex.isValid()) {
        QModelIndex parentIndex = currentIndex.parent();
        int row = currentIndex.row();

        // Проверяем родительский элемент
        if (parentIndex.isValid()) {
            QModelIndex parentNameIndex = sourceModel()->index(row, RegParmItem::T_NAME, parentIndex);
            if (parentNameIndex.isValid()) {
                QString parentName = sourceModel()->data(parentNameIndex).toString();
                if (parentName.contains(m_filterString, Qt::CaseInsensitive)) {
                    return true; // Нашли родителя, который совпал с фильтром
                }
            }
        }

        // Переходим к следующему родителю
        currentIndex = parentIndex;
    }

    return false;
}

bool RegParmProxyModel::nodeMatchesFilter(int source_row, const QModelIndex &source_parent) const
{
    QModelIndex index = sourceModel()->index(source_row, RegParmItem::T_NAME, source_parent);
    if (!index.isValid())
        return false;

    QString name = sourceModel()->data(index).toString();
    return name.contains(m_filterString, Qt::CaseInsensitive);
}

void RegParmProxyModel::setFilterString(const QString &filter)
{
    if (m_filterString != filter) {
        m_filterString = filter.trimmed();
        qDebug() << "Setting filter to:" << m_filterString;
        invalidateFilter();
    }
}

void RegParmProxyModel::setShowOnlyMatches(bool showOnly)
{
    if (m_showOnlyMatches != showOnly) {
        m_showOnlyMatches = showOnly;
        qDebug() << "Setting showOnlyMatches to:" << m_showOnlyMatches;
        invalidateFilter();
    }
}
