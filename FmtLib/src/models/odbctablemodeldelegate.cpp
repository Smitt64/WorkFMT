#include "odbctablemodeldelegate.h"
#include "odbctablemodel.h"
#include <QPainter>
#include <QSortFilterProxyModel>

OdbcTableModelDelegate::OdbcTableModelDelegate(QObject *parent)
    : QStyledItemDelegate{parent}
{

}

OdbcTableModelDelegate::~OdbcTableModelDelegate()
{

}

QSize OdbcTableModelDelegate::sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    return QSize(option.rect.width(), 38);
}

void OdbcTableModelDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    const OdbcTableModel *model = qobject_cast<const OdbcTableModel*>(index.model());

    if (!model)
    {
        const QSortFilterProxyModel *sortModel = qobject_cast<const QSortFilterProxyModel*>(index.model());

        if (sortModel)
            model = qobject_cast<const OdbcTableModel*>(sortModel->sourceModel());
    }
    const OdbcElement &element = model->element(index);

    QStyledItemDelegate::paint(painter, option, index);

    if (!model)
        return;

    painter->save();
    QFont font = option.font;

    int pixelSize = font.pointSize();
    font.setPointSize(pixelSize - 1);
    painter->setPen(Qt::gray);
    painter->setFont(font);
    painter->drawText(option.rect, Qt::AlignRight | Qt::AlignBottom, element._home);

#ifdef Q_OS_WIN
    QString node;
    if (element._node == OdbcTableModel::Node64)
        node = "Wow6432Node\\ODBC.INI";
    else
        node = "ODBC\\ODBC.INI";
    painter->drawText(option.rect, Qt::AlignLeft | Qt::AlignBottom, node);
#endif
    painter->restore();
}
