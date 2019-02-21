#include "fmttablelistdelegate.h"
#include "fmtcore.h"
#include <QApplication>
#include <QPainter>
#include <QTextDocument>
#include <QAbstractTextDocumentLayout>

FmtTableListDelegate::FmtTableListDelegate(QObject *parent) :
    QStyledItemDelegate(parent)
{

}

void FmtTableListDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    QStyleOptionViewItemV4 optionV4 = option;
    QStyleOptionViewItem opt = option;
    initStyleOption(&optionV4, index);

    bool fRenderText = false;
    QString value;
    if (!m_HighlightText.isEmpty())
    {
        QString str = optionV4.text;
        QString rxstring = QString("(%1)").arg(m_HighlightText);
        QRegExp rx(rxstring);
        rx.setCaseSensitivity(Qt::CaseInsensitive);
        value = str.replace(rx, QString("<span style=\" background-color:#9BFF9B;\">\\1</span>"));

        if (value != optionV4.text)
            fRenderText = true;
    }

    QStyle *style = optionV4.widget ? optionV4.widget->style() : QApplication::style();
    if (!fRenderText)
        QStyledItemDelegate::paint(painter, opt, index);
    else
    {
        QTextDocument doc;
        doc.setDocumentMargin(2);
        doc.setDefaultFont(optionV4.font);
        doc.setHtml(value);
        optionV4.text = QString();

        QAbstractTextDocumentLayout::PaintContext ctx;
        style->drawControl(QStyle::CE_ItemViewItem, &optionV4, painter);

        QRect textRect = style->subElementRect(QStyle::SE_ItemViewItemText, &optionV4);
        painter->save();
        painter->translate(textRect.topLeft());
        painter->setClipRect(textRect.translated(-textRect.topLeft()));
        doc.documentLayout()->draw(painter, ctx);
        painter->restore();
    }
}

QString FmtTableListDelegate::highlightText() const
{
    return m_HighlightText;
}

void FmtTableListDelegate::setHighlightText(const QString &text)
{
    m_HighlightText = text;
}
