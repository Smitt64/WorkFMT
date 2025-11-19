#ifndef WORDPREVIEWREGPAGE_H
#define WORDPREVIEWREGPAGE_H

#include <QWizardPage>

namespace Ui {
class WordPreviewRegPage;
}

class WordPreviewRegPage : public QWizardPage
{
    Q_OBJECT

public:
    explicit WordPreviewRegPage(QWidget *parent = nullptr);
    ~WordPreviewRegPage();

    virtual void initializePage() Q_DECL_OVERRIDE;

private:
    Ui::WordPreviewRegPage *ui;
};

#endif // WORDPREVIEWREGPAGE_H
