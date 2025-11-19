#ifndef VIEWDATPAGE_H
#define VIEWDATPAGE_H

#include <QWizardPage>

namespace Ui {
class ViewDatPage;
}

class QKeyEvent;
class RegParmProxyModel;
class ViewDatPage : public QWizardPage
{
    Q_OBJECT

public:
    explicit ViewDatPage(QWidget *parent = nullptr);
    ~ViewDatPage();

    virtual void initializePage() Q_DECL_OVERRIDE;
    virtual bool eventFilter(QObject *obj, QEvent *event) Q_DECL_OVERRIDE;

protected:
    virtual void keyPressEvent(QKeyEvent *event) Q_DECL_OVERRIDE;

private slots:
    void applyFilter();

private:
    Ui::ViewDatPage *ui;
    RegParmProxyModel *m_pProxyModel;
};

#endif // VIEWDATPAGE_H
