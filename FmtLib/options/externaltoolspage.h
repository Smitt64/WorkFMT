#ifndef EXTERNALTOOLSPAGE_H
#define EXTERNALTOOLSPAGE_H

#include <QWidget>
#include <optionspage.h>

namespace Ui {
class ExternalToolsPage;
}

class ExternalToolsPage : public OptionsPage
{
    Q_OBJECT

public:
    ExternalToolsPage(QWidget *parent = nullptr);
    virtual ~ExternalToolsPage();

    virtual int save() Q_DECL_OVERRIDE;
    virtual void restore() Q_DECL_OVERRIDE;

private slots:
    void on_toolButton_clicked();

private:
    Ui::ExternalToolsPage *ui;
};

#endif // EXTERNALTOOLSPAGE_H
