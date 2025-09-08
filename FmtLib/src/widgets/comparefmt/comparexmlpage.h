#ifndef COMPAREXMLPAGE_H
#define COMPAREXMLPAGE_H

#include <QWizardPage>

namespace Ui {
class CompareXmlPage;
}


class CodeEditor;
class CompareXmlPage : public QWizardPage
{
    Q_OBJECT

public:
    explicit CompareXmlPage(QWidget *parent = nullptr);
    ~CompareXmlPage();

private:
    Ui::CompareXmlPage *ui;

    CodeEditor *m_pXmlCodeEditor, *m_pOraCodeEditor, *m_pPgCodeEditor;
};

#endif // COMPAREXMLPAGE_H
