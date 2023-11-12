#ifndef ODBCTABLEMODELDELEGATE_H
#define ODBCTABLEMODELDELEGATE_H

#include <QStyledItemDelegate>
#include <QObject>
#include "fmtlib_global.h"

class FMTLIBSHARED_EXPORT OdbcTableModelDelegate : public QStyledItemDelegate
{
public:
    OdbcTableModelDelegate(QObject *parent = nullptr);
    virtual ~OdbcTableModelDelegate();

    virtual QSize sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const Q_DECL_FINAL;
    virtual void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const Q_DECL_FINAL;
};

#endif // ODBCTABLEMODELDELEGATE_H
