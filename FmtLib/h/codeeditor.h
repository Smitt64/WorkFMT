#ifndef CODEEDITOR_H
#define CODEEDITOR_H

#include <QPlainTextEdit>
#include <QObject>
#include "highlighter.h"
#include "fmtcore.h"

#define TABSTOP 4

class QPaintEvent;
class QResizeEvent;
class QSize;
class QWidget;

class LineNumberArea;
class QAbstractItemModel;
class FMTLIBSHARED_EXPORT CodeEditor : public QPlainTextEdit
{
    Q_OBJECT

public:
    CodeEditor(QWidget *parent = 0);
    ~CodeEditor();

    void lineNumberAreaPaintEvent(QPaintEvent *event);
    int lineNumberAreaWidth();

    void setTabStop(int w);
    void resetStyle();

signals:
    void currentLineChanged(const int &last, const int &cur);

protected:
    void resizeEvent(QResizeEvent *event) Q_DECL_OVERRIDE;

protected:
    void focusInEvent(QFocusEvent *e) Q_DECL_OVERRIDE;

private slots:
    void updateLineNumberAreaWidth(int newBlockCount);
    void highlightCurrentLine();
    void updateLineNumberArea(const QRect &, int);
    void onSelectionChanged();

private:
    QWidget *lineNumberArea;
    QTextEdit::ExtraSelection selectionCurrentLine;
    QList<QTextEdit::ExtraSelection> Selections, applyExtraSelections;

    void ApplyExtraSelections();
    int tabStop, lastLine;

    QColor m_CurrentLineColor, m_CurrentWordColor;
};


class LineNumberArea : public QWidget
{
public:
    LineNumberArea(CodeEditor *editor) : QWidget(editor) {
        codeEditor = editor;
    }
    ~LineNumberArea()
    {
        codeEditor = NULL;
    }

    QSize sizeHint() const Q_DECL_OVERRIDE {
        return QSize(codeEditor->lineNumberAreaWidth(), 0);
    }

protected:
    void paintEvent(QPaintEvent *event) Q_DECL_OVERRIDE {
        codeEditor->lineNumberAreaPaintEvent(event);
    }

private:
    CodeEditor *codeEditor;
};

#endif // CODEEDITOR_H
