#include "massdestribitemparamdelegate.h"
#include "massdestribparammodel.h"
#include <QPainter>
#include <QCheckBox>

MassDestribItemParamDelegate::MassDestribItemParamDelegate(QObject *parent) :
    QStyledItemDelegate(parent)
{

}

MassDestribItemParamDelegate::~MassDestribItemParamDelegate()
{

}

QWidget *MassDestribItemParamDelegate::createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    Q_UNUSED(option);
    if (!index.parent().isValid())
    {
        if (index.column() == MassDestribParamModel::fld_Action)
        {
            const MassDestribParamModel *pModel = qobject_cast<const MassDestribParamModel*>(index.model());
            if (pModel == Q_NULLPTR)
                return Q_NULLPTR;

            QComboBox *pAction = new QComboBox(parent);
            pAction->addItems(pModel->getTablesActions());
            return pAction;
        }
        else if (index.column() == MassDestribParamModel::fld_UnloadFmt ||
                 index.column() == MassDestribParamModel::fld_UnloadCreateTables ||
                 index.column() == MassDestribParamModel::fld_UnloadDat)
        {
            QCheckBox *check = new QCheckBox(tr("Да"), parent);
            return check;
        }
    }
    else {
        if (index.column() == MassDestribParamModel::fld_Action)
        {
            const MassDestribParamModel *pModel = qobject_cast<const MassDestribParamModel*>(index.model());
            if (pModel == Q_NULLPTR)
                return Q_NULLPTR;

            QComboBox *pAction = new QComboBox(parent);
            pAction->addItems(pModel->getFieldsActions());
            return pAction;
        }
    }
    return Q_NULLPTR;
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

void MassDestribItemParamDelegate::setEditorData(QWidget *editor, const QModelIndex &index) const
{
    if (!index.parent().isValid())
    {
        if (index.column() == MassDestribParamModel::fld_Action)
        {
            QComboBox *pAction = qobject_cast<QComboBox*>(editor);
            pAction->setCurrentIndex(index.data(Qt::EditRole).value<int>());
        }
        else {
            QCheckBox *pAction = qobject_cast<QCheckBox*>(editor);
            pAction->setChecked(index.data(Qt::EditRole).toBool());
        }
    }
    else {
        if (index.column() == MassDestribParamModel::fld_Action)
        {
            QComboBox *pAction = qobject_cast<QComboBox*>(editor);
            pAction->setCurrentIndex(index.data(Qt::EditRole).value<int>());
        }
    }
}

void MassDestribItemParamDelegate::setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const
{
    if (!index.parent().isValid())
    {
        if (index.column() == MassDestribParamModel::fld_Action)
        {
            QComboBox *pAction = qobject_cast<QComboBox*>(editor);
            model->setData(index, pAction->currentIndex(), Qt::EditRole);
        }
        else {
            QCheckBox *pAction = qobject_cast<QCheckBox*>(editor);
            model->setData(index, pAction->isChecked(), Qt::EditRole);
        }
    }
    else {
        if (index.column() == MassDestribParamModel::fld_Action)
        {
            QComboBox *pAction = qobject_cast<QComboBox*>(editor);
            model->setData(index, pAction->currentIndex(), Qt::EditRole);
        }
    }
}
