#include "fmtindecesdelegate.h"
#include "fmtindecesmodel.h"
#include "fmtindecesmodelitem.h"
#include "fmttable.h"
#include "fmtcore.h"
#include <QPainter>
#include <QtWidgets>

FmtIndecesDelegate::FmtIndecesDelegate(QWidget *parent) :
    QStyledItemDelegate(parent)
{
}

void FmtIndecesDelegate::setFmtTable(FmtTable *table)
{
    pTable = table;
}

QSize FmtIndecesDelegate::sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    Q_UNUSED(index);
    QSize size(option.rect.width(), 25);
    return size;
}

void FmtIndecesDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    QStyleOptionViewItem opt = option;
    if (!index.parent().isValid())
    {
        painter->fillRect(option.rect, QColor(Qt::yellow).lighter(165));

        if (((opt.state & QStyle::State_MouseOver) || option.rect.contains(m_Point)) && index.column() == 0)
        {
            opt.rect.setX(BTNS_OFFSET);
        }
    }
    else
    {
        int col = index.column();
        if (col == FmtIndecesModelItem::fld_Dup || col == FmtIndecesModelItem::fld_Type ||
                col == FmtIndecesModelItem::fld_Null || col == FmtIndecesModelItem::fld_AutoInc ||
                col == FmtIndecesModelItem::fld_Local)
        {
            painter->fillRect(opt.rect, Qt::lightGray);
        }
    }

    if (index.parent().isValid())
    {
        QPen pen;
        pen.setStyle(Qt::DotLine);

        painter->save();
        painter->setPen(pen);
        painter->drawLine(opt.rect.topRight(), opt.rect.bottomRight());

        pen.setStyle(Qt::DotLine);
        pen.setColor(Qt::lightGray);
        painter->setPen(pen);
        painter->drawLine(opt.rect.bottomLeft(), opt.rect.bottomRight());

        if (index.column() == 0)
        {
            painter->drawLine(opt.rect.bottomLeft(), opt.rect.topLeft());
        }
        painter->restore();
    }

    painter->save();
    QPen pen;
    pen.setStyle(Qt::DotLine);
    pen.setColor(Qt::lightGray);
    painter->setPen(pen);
    painter->drawLine(QPoint(opt.rect.bottomLeft().x() - BTNS_OFFSET, opt.rect.bottomLeft().y()), opt.rect.bottomRight());
    painter->restore();

    if (index.parent().isValid() && index.column() == FmtIndecesModelItem::fld_Panel)
    {
        QStyleOptionButton button;
        button.rect = option.rect;
        button.text = "...";
        button.state = option.state;
        QApplication::style()->drawControl(QStyle::CE_PushButton, &button, painter);
        return;
    }

    QStyledItemDelegate::paint(painter, opt, index);
}

QWidget *FmtIndecesDelegate::createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    if (index.parent().isValid())
    {
        if (index.column() == FmtIndecesModelItem::fld_Name)
        {
            QComboBox *combo = new QComboBox(parent);
            combo->setMaxVisibleItems(14);
            combo->setModel((QAbstractItemModel*)pTable->fieldsModel());
            return combo;
        }

        if (index.column() == FmtIndecesModelItem::fld_Desc || index.column() == FmtIndecesModelItem::fld_ExcIndx || index.column() == FmtIndecesModelItem::fld_NotNull)
        {
            QCheckBox *check = new QCheckBox(tr("Да"),parent);
            return check;
        }

        if (index.column() == FmtIndecesModelItem::fld_Comment)
        {
            QLineEdit *lineEdit = new QLineEdit(parent);
            return lineEdit;
        }
    }
    else
    {
        if (index.column() == FmtIndecesModelItem::fld_AutoInc || index.column() == FmtIndecesModelItem::fld_Dup || index.column() == FmtIndecesModelItem::fld_Local)
        {
            QCheckBox *check = new QCheckBox(tr("Да"), parent);
            return check;
        }

        if (index.column() == FmtIndecesModelItem::fld_Null)
        {
            QComboBox *combo = new QComboBox(parent);
            combo->addItem("");
            combo->addItem("All");
            combo->addItem("Any");
            return combo;
        }
    }
    return QStyledItemDelegate::createEditor(parent, option, index);
}

void FmtIndecesDelegate::setEditorData(QWidget *editor, const QModelIndex &index) const
{
    if (!index.parent().isValid())
    {
        if (index.column() == FmtIndecesModelItem::fld_Dup || index.column() == FmtIndecesModelItem::fld_AutoInc || index.column() == FmtIndecesModelItem::fld_Local)
        {
            QCheckBox *check = qobject_cast<QCheckBox*>(editor);
            FmtIndecesModelItem *item = static_cast<FmtIndecesModelItem*>(index.internalPointer());
            check->setChecked(item->data(index.column(), Qt::EditRole).toBool());
        }

        if (index.column() == FmtIndecesModelItem::fld_Null)
        {
            QComboBox *combo = qobject_cast<QComboBox*>(editor);
            FmtIndecesModelItem *item = static_cast<FmtIndecesModelItem*>(index.internalPointer());
            combo->setCurrentIndex(item->data(index.column(), Qt::EditRole).toInt());
        }
    }
    else
    {
        if (index.column() == FmtIndecesModelItem::fld_NotNull || index.column() == FmtIndecesModelItem::fld_Desc
                || index.column() == FmtIndecesModelItem::fld_ExcIndx)
        {
            QCheckBox *check = qobject_cast<QCheckBox*>(editor);
            FmtIndecesModelItem *item = static_cast<FmtIndecesModelItem*>(index.internalPointer());
            bool checked = item->data(index.column(), Qt::EditRole).toBool();
            check->setChecked(checked);
        }

        if (index.column() == FmtIndecesModelItem::fld_Comment)
        {
            QLineEdit *lineEdit = qobject_cast<QLineEdit*>(editor);
            FmtIndecesModelItem *item = static_cast<FmtIndecesModelItem*>(index.internalPointer());
            lineEdit->setText(item->data(index.column(), Qt::EditRole).toString());
        }

        if (index.column() == FmtIndecesModelItem::fld_Name)
        {
            QComboBox *combo = qobject_cast<QComboBox*>(editor);
            FmtIndecesModelItem *item = static_cast<FmtIndecesModelItem*>(index.internalPointer());
            combo->setCurrentIndex(item->data(index.column(), Qt::EditRole).toInt());
        }
    }
    //QStyledItemDelegate::setEditorData(editor, index);
}

void FmtIndecesDelegate::setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const
{
    if (!index.parent().isValid())
    {
        if (index.column() == FmtIndecesModelItem::fld_Dup || index.column() == FmtIndecesModelItem::fld_AutoInc ||
                index.column() == FmtIndecesModelItem::fld_Local)
        {
            QCheckBox *check = qobject_cast<QCheckBox*>(editor);
            FmtIndecesModelItem *item = static_cast<FmtIndecesModelItem*>(index.internalPointer());
            item->setData(index.column(), check->isChecked());
        }

        if (index.column() == FmtIndecesModelItem::fld_Null)
        {
            QComboBox *combo = qobject_cast<QComboBox*>(editor);
            FmtIndecesModelItem *item = static_cast<FmtIndecesModelItem*>(index.internalPointer());
            item->setData(index.column(), combo->currentIndex());
        }
    }
    else
    {
        if (index.column() == FmtIndecesModelItem::fld_NotNull)
        {
            QCheckBox *check = qobject_cast<QCheckBox*>(editor);
            FmtIndecesModelItem *item = static_cast<FmtIndecesModelItem*>(index.internalPointer());
            item->setData(index.column(), check->isChecked());
        }
        if (index.column() == FmtIndecesModelItem::fld_Desc
                || index.column() == FmtIndecesModelItem::fld_ExcIndx)
        {
            QCheckBox *check = qobject_cast<QCheckBox*>(editor);
            FmtIndecesModelItem *item = static_cast<FmtIndecesModelItem*>(index.internalPointer());
            item->setData(index.column(), check->isChecked());
        }

        if (index.column() == FmtIndecesModelItem::fld_Name)
        {
            QComboBox *combo = qobject_cast<QComboBox*>(editor);
            FmtIndecesModelItem *item = static_cast<FmtIndecesModelItem*>(index.internalPointer());
            item->setData(index.column(), combo->currentIndex());
        }

        if (index.column() == FmtIndecesModelItem::fld_Comment)
        {
            QLineEdit *lineEdit = qobject_cast<QLineEdit*>(editor);
            FmtIndecesModelItem *item = static_cast<FmtIndecesModelItem*>(index.internalPointer());
            item->setData(index.column(), lineEdit->text());
        }
    }

    QStyledItemDelegate::setModelData(editor, model, index);
}

void FmtIndecesDelegate::setPoint(const QPoint &pt)
{
    m_Point = pt;
}
