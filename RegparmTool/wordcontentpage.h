#ifndef WORDCONTENTPAGE_H
#define WORDCONTENTPAGE_H

#include <QWizardPage>

namespace Ui {
class WordContentPage;
}

class QTextDocument;
class WordContentPage : public QWizardPage
{
    Q_OBJECT

public:
    explicit WordContentPage(QWidget *parent = nullptr);
    ~WordContentPage();

    QTextDocument *document();

private:
    Ui::WordContentPage *ui;
};

#endif // WORDCONTENTPAGE_H
