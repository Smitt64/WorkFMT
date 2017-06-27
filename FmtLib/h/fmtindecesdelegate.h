#ifndef FMTINDECESDELEGATE_H
#define FMTINDECESDELEGATE_H

#include <QWidget>
#include <QStyledItemDelegate>

class FmtTable;
class QComboBox;
class FmtIndecesDelegate : public QStyledItemDelegate
{
public:
    FmtIndecesDelegate(QWidget *parent = 0);

    virtual QSize sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const;
    virtual void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const;
    virtual QWidget *createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const;
    virtual void setEditorData(QWidget *editor, const QModelIndex &index) const;
    virtual void setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const;

    void setFmtTable(FmtTable *table);
    void setPoint(const QPoint &pt);
private:
    FmtTable *pTable;
    QPoint m_Point;
};

#endif // FMTINDECESDELEGATE_H
