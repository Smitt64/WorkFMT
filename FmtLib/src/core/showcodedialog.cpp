#include <QWidget>
#include <QDialog>
#include <QVBoxLayout>
#include <QSyntaxHighlighter>
#include <codeeditor/codehighlighter.h>
#include <codeeditor/codeeditor.h>

int ShowCodeDialog(QWidget *parent, const QString &title, const int &type, const QString &code)
{
    int result = 0;
    QDialog dlg(parent);
    //QSyntaxHighlighter *pCurrentHighlighter = Q_NULLPTR;
    CodeEditor *editor = new CodeEditor(&dlg);
    QVBoxLayout *main = new QVBoxLayout(&dlg);
    editor->setReadOnly(true);

    // HighlighterSql
    ToolApplyHighlighter(editor, type);

    main->addWidget(editor);
    dlg.setMinimumSize(640, 480);
    dlg.setWindowTitle(title);
    dlg.setLayout(main);

    editor->setPlainText(code);

    result = dlg.exec();

    return result;
}
