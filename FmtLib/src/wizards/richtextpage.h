#ifndef RICHTEXTPAGE_H
#define RICHTEXTPAGE_H

#include <QWizardPage>

namespace Ui {
class RichTextPage;
}

class QTextDocument;
class RichTextPage : public QWizardPage
{
    Q_OBJECT

public:
    explicit RichTextPage(QWidget *parent = nullptr);
    ~RichTextPage();

    void setDocument(QTextDocument *document);

    bool isComplete() const Q_DECL_OVERRIDE;

private slots:
    void onTextChanged();

private:
    Ui::RichTextPage *ui;
};

#endif // RICHTEXTPAGE_H
