#ifndef FMTFIELDSTABLEHEADERDELEGATE_H
#define FMTFIELDSTABLEHEADERDELEGATE_H

#include <QtWidgets>

class FmtFieldsTableHeaderDelegate : public QHeaderView
{
    Q_OBJECT
public:
    FmtFieldsTableHeaderDelegate(Qt::Orientation orientation, QWidget *parent = Q_NULLPTR);
    //virtual void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const;

protected:
    virtual void paintSection(QPainter *painter, const QRect &rect, int logicalIndex) const;
};

#endif // FMTFIELDSTABLEHEADERDELEGATE_H
