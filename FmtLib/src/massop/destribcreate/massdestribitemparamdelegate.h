#ifndef MASSDESTRIBITEMPARAMDELEGATE_H
#define MASSDESTRIBITEMPARAMDELEGATE_H

#include <QStyledItemDelegate>

class MassDestribItemParamDelegate : public QStyledItemDelegate
{
public:
    MassDestribItemParamDelegate(QObject *parent = Q_NULLPTR);
    virtual ~MassDestribItemParamDelegate();

    QWidget *createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const Q_DECL_OVERRIDE;
    void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const Q_DECL_OVERRIDE;
    void setEditorData(QWidget *editor, const QModelIndex &index) const Q_DECL_OVERRIDE;
    void setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const Q_DECL_OVERRIDE;

};

#endif // MASSDESTRIBITEMPARAMDELEGATE_H
