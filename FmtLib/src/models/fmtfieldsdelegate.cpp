#include "fmtfieldsdelegate.h"
#include "fmtfildsmodel.h"
#include "fmtcore.h"
#include <QPainter>
#include <QComboBox>
#include <QApplication>
#include <QLineEdit>

#define DEFAULT_HEIGHT 25

FmtFieldsDelegate::FmtFieldsDelegate(QWidget *parent) :
    QStyledItemDelegate(parent)
{

}

QSize FmtFieldsDelegate::sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    return QStyledItemDelegate::sizeHint(option, index);
}

void FmtFieldsDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    QStyleOptionViewItem opt = option;
    if (index.column() == FmtFildsModel::fld_Custom)
    {
        QStyleOptionButton button;
        button.rect = option.rect;
        button.text = "...";
        button.state = option.state;

        if (index.row() >= index.model()->rowCount() - 1)
        {
            button.state &= ~QStyle::State_Enabled;
        }
        QApplication::style()->drawControl(QStyle::CE_PushButton, &button, painter);
        return;
    }

    if (index.column() == FmtFildsModel::fld_DbName)
    {
       painter->fillRect(option.rect, Qt::lightGray);
    }

    QStyledItemDelegate::paint(painter, opt, index);
}

QWidget *FmtFieldsDelegate::createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    if (index.column() == FmtFildsModel::fld_Type)
    {
        QComboBox *combo = new QComboBox(parent);
        combo->setMaxVisibleItems(14);
        combo->addItems(fmtTypes());
        return combo;
    }
    if (index.column() == FmtFildsModel::fld_Comment || index.column() == FmtFildsModel::fld_Name)
    {
        QLineEdit *edit = new QLineEdit(parent);

        int maxLen = fmtm_FieldNameMaxSize;
        if (index.column() == FmtFildsModel::fld_Comment)
            maxLen = fmtm_FieldCommentMaxSize;

        edit->setMaxLength(maxLen);
        return edit;
    }
    return QStyledItemDelegate::createEditor(parent, option, index);
}

void FmtFieldsDelegate::setEditorData(QWidget *editor, const QModelIndex &index) const
{
    if (index.column() == FmtFildsModel::fld_Type)
    {
        QComboBox *combo = qobject_cast<QComboBox*>(editor);
        combo->setCurrentIndex(fmtTypeIndexForId(index.data(Qt::EditRole).toInt()));
    }
    if (index.column() == FmtFildsModel::fld_Comment || index.column() == FmtFildsModel::fld_Name)
    {
        QLineEdit *edit = qobject_cast<QLineEdit*>(editor);
        edit->setText(index.data(Qt::EditRole).toString());
    }
    else QStyledItemDelegate::setEditorData(editor, index);
}

void FmtFieldsDelegate::setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const
{
    if (index.column() == FmtFildsModel::fld_Type)
    {
        QComboBox *combo = qobject_cast<QComboBox*>(editor);
        model->setData(index, fmtTypeFromIndex(combo->currentIndex()), Qt::EditRole);

        if (fmtTypeIndexSize(combo->currentIndex()) != 0)
            model->setData(model->index(index.row(), FmtFildsModel::fld_Size), fmtTypeIndexSize(combo->currentIndex()));
    }
    else if (index.column() == FmtFildsModel::fld_Comment || index.column() == FmtFildsModel::fld_Name)
    {
        QLineEdit *edit = qobject_cast<QLineEdit*>(editor);
        model->setData(index, edit->text(), Qt::EditRole);
    }
    else
        QStyledItemDelegate::setModelData(editor, model, index);
}
