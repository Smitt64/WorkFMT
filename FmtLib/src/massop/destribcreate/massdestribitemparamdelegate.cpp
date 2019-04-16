#include "massdestribitemparamdelegate.h"
#include "massdestribparammodel.h"
#include <QPainter>

MassDestribItemParamDelegate::MassDestribItemParamDelegate(QObject *parent) :
    QStyledItemDelegate(parent)
{

}

MassDestribItemParamDelegate::~MassDestribItemParamDelegate()
{

}

void MassDestribItemParamDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    QStyledItemDelegate::paint(painter, option, index);

    if (!index.parent().isValid())
    {
        painter->setPen(Qt::DashLine);
        painter->drawLine(QPoint(0, option.rect.bottom()), QPoint(option.rect.right(), option.rect.bottom()));
    }
    else
    {
        if (index.column() == MassDestribParamModel::fld_Name)
        {
            painter->setPen(Qt::DashLine);
            painter->drawLine(QPoint(option.rect.right(), 0), QPoint(option.rect.right(), option.rect.bottom()));
        }
    }
}
