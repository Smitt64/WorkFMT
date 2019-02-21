#include <QWidget>
#include <QDialog>
#include <QVBoxLayout>
#include <QSyntaxHighlighter>
#include "highlighter.h"
#include "codeeditor.h"
#include "fmtgeninterface.h"

int ShowCodeDialog(QWidget *parent, const QString &title, const int &type, const QString &code)
{
    int result = 0;
    QDialog dlg(parent);
    QSyntaxHighlighter *pCurrentHighlighter = Q_NULLPTR;
    CodeEditor *editor = new CodeEditor(&dlg);
    QVBoxLayout *main = new QVBoxLayout(&dlg);
    editor->setReadOnly(true);

    switch(type)
    {
    case FmtGenInterface::GenSql:
        pCurrentHighlighter = new SqlHighlighter(editor->document());
        break;
    case FmtGenInterface::GenCpp:
        pCurrentHighlighter = new Highlighter(editor->document());
        break;
    }

    main->addWidget(editor);
    dlg.setMinimumSize(640, 480);
    dlg.setWindowTitle(title);
    dlg.setLayout(main);

    editor->setPlainText(code);

    result = dlg.exec();

    return result;
}
