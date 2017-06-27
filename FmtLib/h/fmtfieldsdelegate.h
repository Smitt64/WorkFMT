#ifndef FMTFIELDSDELEGATE_H
#define FMTFIELDSDELEGATE_H

#include <QStyledItemDelegate>

class FmtFieldsDelegate : public QStyledItemDelegate
{
public:
    FmtFieldsDelegate(QWidget *parent = 0);

    virtual QSize sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const;
    virtual void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const;
    virtual QWidget *createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const;
    virtual void setEditorData(QWidget *editor, const QModelIndex &index) const;
    virtual void setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const;
};

#endif // FMTFIELDSDELEGATE_H
