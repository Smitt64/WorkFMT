#include "fmtfieldstableheaderdelegate.h"

FmtFieldsTableHeaderDelegate::FmtFieldsTableHeaderDelegate(Qt::Orientation orientation, QWidget *parent) :
    QHeaderView(orientation, parent)
{

}

void FmtFieldsTableHeaderDelegate::paintSection(QPainter *painter, const QRect &rect, int logicalIndex) const
{
    QAbstractItemModel *mdl = model();
    /*QStyleOptionViewItem opt = option;
    //opt.decorationAlignment = Qt::AlignCenter;
    //opt.decorationPosition = QStyleOptionViewItem::Top;
    opt.icon = QIcon();
    QStyledItemDelegate::paint(painter, opt, index);

    const QStyle* style = option.widget ? option.widget->style() : qApp->style();
    style->drawItemPixmap(painter, option.rect, Qt::AlignCenter, option.icon.pixmap(option.rect.size()));
    qDebug() << "paint";*/
    QString text = mdl->headerData(logicalIndex, Qt::Horizontal).toString();
    QPixmap icon = mdl->headerData(logicalIndex, Qt::Horizontal, Qt::DecorationRole).value<QPixmap>();

    if (!text.isEmpty())
        QHeaderView::paintSection(painter, rect, logicalIndex);
    else
    {
        if (icon.isNull())
            QHeaderView::paintSection(painter, rect, logicalIndex);
        else
        {
            QStyleOptionHeader opt;
            opt.init(this);
            opt.rect = rect;
            style()->drawControl(QStyle::CE_HeaderSection, &opt, painter);
            style()->drawItemPixmap(painter, rect, Qt::AlignCenter, icon.scaled(QSize(16, 16), Qt::KeepAspectRatio, Qt::SmoothTransformation));
        }
    }
}
