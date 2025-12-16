#ifndef WORDPREVIEWREGPAGE_H
#define WORDPREVIEWREGPAGE_H

#include <QWizardPage>
#include <QSharedPointer>
#include "rsl/reginfoobj.h"

namespace Ui {
class WordPreviewRegPage;
}

class WordPreviewRegPage : public QWizardPage
{
    Q_OBJECT

public:
    explicit WordPreviewRegPage(QWidget *parent = nullptr);
    ~WordPreviewRegPage();

    void initializePage() override;

    QSharedPointer<RegInfoObjModel> model();

private:
    void setupTreeView();
    void adjustColumnsAfterDataLoad();

    Ui::WordPreviewRegPage *ui;
    QSharedPointer<RegInfoObjModel> m_pModel;
};

#endif // WORDPREVIEWREGPAGE_H
