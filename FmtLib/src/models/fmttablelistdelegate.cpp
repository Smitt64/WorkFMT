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
    QStyleOptionViewItem optionV4 = option;
    initStyleOption(&optionV4, index);

    bool fRenderText = false;
    QString value;
    if (!m_HighlightText.isEmpty())
    {
        QString str = optionV4.text;
        QString rxstring = QString("(%1)").arg(QRegExp::escape(m_HighlightText));
        QRegExp rx(rxstring);
        rx.setCaseSensitivity(Qt::CaseInsensitive);
        value = str.replace(rx, QString("<span style=\"background-color:#9BFF9B;\">\\1</span>"));

        if (value != optionV4.text)
            fRenderText = true;
    }

    if (!fRenderText)
        QStyledItemDelegate::paint(painter, option, index);
    else
    {
        QStyle *style = optionV4.widget ? optionV4.widget->style() : QApplication::style();

        optionV4.text = QString();
        optionV4.state &= ~QStyle::State_HasFocus;

        style->drawControl(QStyle::CE_ItemViewItem, &optionV4, painter, optionV4.widget);

        QTextDocument doc;
        doc.setDocumentMargin(0.5);
        doc.setDefaultFont(optionV4.font);
        doc.setHtml(value);

        QRect textRect = style->subElementRect(QStyle::SE_ItemViewItemText, &optionV4, optionV4.widget);
        textRect.adjust(2, 0, -2, 0);
        painter->save();

        painter->translate(textRect.topLeft());
        painter->setClipRect(QRect(0, 0, textRect.width(), textRect.height()));

        QAbstractTextDocumentLayout::PaintContext ctx;
        ctx.palette.setColor(QPalette::Text, optionV4.palette.color(QPalette::Text));
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
