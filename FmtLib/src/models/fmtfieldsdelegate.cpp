#include "fmtfieldsdelegate.h"
#include "fmtfildsmodel.h"
#include "fmtcore.h"
#include <QPainter>
#include <QComboBox>
#include <QApplication>
#include <QLineEdit>
#include <QTextDocument>
#include <QAbstractTextDocumentLayout>

#define DEFAULT_HEIGHT 25

FmtFieldsDelegate::FmtFieldsDelegate(QObject *parent) :
    QStyledItemDelegate(parent)
{

}

FmtFieldsDelegate::~FmtFieldsDelegate()
{

}

QSize FmtFieldsDelegate::sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    return QStyledItemDelegate::sizeHint(option, index);
}

void FmtFieldsDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    FmtFildsModel *pModel = (FmtFildsModel*)index.model();
    QStyleOptionViewItemV4 optionV4 = option;
    QStyleOptionViewItem opt = option;
    initStyleOption(&optionV4, index);
    QStyle *style = optionV4.widget ? optionV4.widget->style() : QApplication::style();

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

    QTextDocument doc;
    doc.setDefaultFont(optionV4.font);

    QVariant text = optionV4.text;
    if (index.column() == FmtFildsModel::fld_Name || index.column() == FmtFildsModel::fld_Comment)
    {
        if (index.column() == FmtFildsModel::fld_Name && !m_NameText.isEmpty())
            text = pModel->ProcessHighlightFields(index, FmtFildsModel::FindSubTextRole, m_NameText);

        if (index.column() == FmtFildsModel::fld_Comment && !m_CommentText.isEmpty())
            text = pModel->ProcessHighlightFields(index, FmtFildsModel::FindSubTextRole, m_CommentText);

        /*if (!m_HighlightText.isEmpty())
            text = pModel->ProcessHighlightFieldsEx(text.toString(), m_HighlightText, "fff19b");*/
    }
    /*if(index.column() == FmtFildsModel::fld_Name || index.column() == FmtFildsModel::fld_Comment && !m_HighlightText.isEmpty())
        text = pModel->ProcessHighlightFields(index, FmtFildsModel::FindSubTextRole, m_HighlightText);
    else
        text = optionV4.text;*/

    doc.setHtml(text.toString());
    optionV4.text = QString();
    style->drawControl(QStyle::CE_ItemViewItem, &optionV4, painter);

    QAbstractTextDocumentLayout::PaintContext ctx;
    QRect textRect = style->subElementRect(QStyle::SE_ItemViewItemText, &optionV4);
    painter->save();
    painter->translate(textRect.topLeft());
    painter->setClipRect(textRect.translated(-textRect.topLeft()));
    doc.documentLayout()->draw(painter, ctx);
    painter->restore();
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

        int Type = fmtTypeFromIndex(combo->currentIndex());
        model->setData(index, Type, Qt::EditRole);

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

void FmtFieldsDelegate::setHighlightText(const QString &str)
{
    m_HighlightText = str;
}

void FmtFieldsDelegate::setNameText(const QString &str)
{
    m_NameText = str;
}

void FmtFieldsDelegate::setCommentText(const QString &str)
{
    m_CommentText = str;
}
