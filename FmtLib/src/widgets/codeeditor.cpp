#include "codeeditor.h"
#include <QtWidgets>
#include <QRegExp>
#include <QLinearGradient>
#include <QRegExp>
#include <QFontMetrics>

#define COMPLETER_START_LEN 2

CodeEditor::CodeEditor(QWidget *parent) : QPlainTextEdit(parent),
    lastLine(0)
{
    lineNumberArea = new LineNumberArea(this);
    setWordWrapMode(QTextOption::NoWrap);
    connect(this, SIGNAL(blockCountChanged(int)), this, SLOT(updateLineNumberAreaWidth(int)));
    connect(this, SIGNAL(updateRequest(QRect,int)), this, SLOT(updateLineNumberArea(QRect,int)));
    connect(this, SIGNAL(cursorPositionChanged()), this, SLOT(highlightCurrentLine()));
    connect(this, SIGNAL(selectionChanged()), this, SLOT(onSelectionChanged()));

    updateLineNumberAreaWidth(0);
    highlightCurrentLine();
}

CodeEditor::~CodeEditor()
{
    delete lineNumberArea;
}

void CodeEditor::setTabStop(int w)
{
    tabStop = w;
}

void CodeEditor::focusInEvent(QFocusEvent *e)
{
    QPlainTextEdit::focusInEvent(e);
}

void CodeEditor::onSelectionChanged()
{ 
    QTextCursor current = textCursor();

    if (lastLine != current.block().blockNumber())
    {
        emit currentLineChanged(lastLine, current.block().blockNumber());
        lastLine = current.block().blockNumber();
    }

    QString searchString = current.selectedText();
    searchString = searchString.simplified();
    Selections.clear();
    if (!searchString.isEmpty())
    {
        QBrush backBrush("#9BFF9B");

        QTextCursor highlightCursor(document());
        while (!highlightCursor.isNull() && !highlightCursor.atEnd()) {
            highlightCursor = document()->find(searchString, highlightCursor, QTextDocument::FindWholeWords);

            if (!highlightCursor.isNull())
            {
                QTextEdit::ExtraSelection selection;
                selection.cursor = highlightCursor;
                selection.format.setBackground(backBrush);
                Selections.append(selection);
            }
        }
    }

    ApplyExtraSelections();
}

void CodeEditor::ApplyExtraSelections()
{
    QList<QTextEdit::ExtraSelection> selections;
    selections.append(selectionCurrentLine);
    selections.append(Selections);
    setExtraSelections(selections);
}

int CodeEditor::lineNumberAreaWidth()
{
    int digits = 1;
    int max = qMax(4, blockCount());
    while (max >= 10) {
        max /= 10;
        ++digits;
    }

    int space = 1 + fontMetrics().width(QLatin1Char('9')) * /*digits*/5;

    return space;
}

void CodeEditor::updateLineNumberAreaWidth(int /* newBlockCount */)
{
    setViewportMargins(lineNumberAreaWidth(), 0, 0, 0);
}

void CodeEditor::updateLineNumberArea(const QRect &rect, int dy)
{
    if (dy)
        lineNumberArea->scroll(0, dy);
    else
        lineNumberArea->update(0, rect.y(), lineNumberArea->width(), rect.height());

    if (rect.contains(viewport()->rect()))
        updateLineNumberAreaWidth(0);
}

void CodeEditor::resizeEvent(QResizeEvent *e)
{
    QPlainTextEdit::resizeEvent(e);

    QRect cr = contentsRect();
    lineNumberArea->setGeometry(QRect(cr.left(), cr.top(), lineNumberAreaWidth(), cr.height()));
}

void CodeEditor::highlightCurrentLine()
{
    QColor lineColor = QColor(Qt::yellow).lighter(160);

    selectionCurrentLine.format.setBackground(lineColor);
    selectionCurrentLine.format.setProperty(QTextFormat::FullWidthSelection, true);
    selectionCurrentLine.cursor = textCursor();
    selectionCurrentLine.cursor.clearSelection();

    ApplyExtraSelections();
}

void CodeEditor::lineNumberAreaPaintEvent(QPaintEvent *event)
{
    QPainter painter(lineNumberArea);
    painter.fillRect(event->rect(), Qt::lightGray);


    QTextBlock block = firstVisibleBlock();
    int blockNumber = block.blockNumber();
    int top = (int) blockBoundingGeometry(block).translated(contentOffset()).top();
    int bottom = top + (int) blockBoundingRect(block).height();

    while (block.isValid() && top <= event->rect().bottom())
    {
        if (block.isVisible() && bottom >= event->rect().top())
        {
            QFont f = painter.font();
            f.setBold(false);
            QString number = QString::number(blockNumber + 1);
            painter.setPen(Qt::black);

            if (block == this->textCursor().block())
            {
                painter.setPen(QColor(Qt::red).darker());
                f.setBold(true);
            }

            painter.setFont(f);
            painter.drawText(0, top, lineNumberArea->width() - 4, fontMetrics().height(),
                             Qt::AlignRight, number);
        }

        block = block.next();
        top = bottom;
        bottom = top + (int) blockBoundingRect(block).height();
        ++blockNumber;
    }
}
