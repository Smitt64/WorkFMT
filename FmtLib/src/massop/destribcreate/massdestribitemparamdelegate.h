#ifndef MASSDESTRIBITEMPARAMDELEGATE_H
#define MASSDESTRIBITEMPARAMDELEGATE_H

#include <QStyledItemDelegate>

class MassDestribItemParamDelegate : public QStyledItemDelegate
{
public:
    MassDestribItemParamDelegate(QObject *parent = Q_NULLPTR);
    virtual ~MassDestribItemParamDelegate();

    void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const Q_DECL_OVERRIDE;
};

#endif // MASSDESTRIBITEMPARAMDELEGATE_H
