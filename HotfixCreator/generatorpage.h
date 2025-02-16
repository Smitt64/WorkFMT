#ifndef GENERATORPAGE_H
#define GENERATORPAGE_H

#include <QWizardPage>

class ErrorDlg;
class QVBoxLayout;
class QProgressBar;
class ErrorsModel;
class GeneratorPage : public QWizardPage
{
    Q_OBJECT
public:
    GeneratorPage(QWidget *parent = nullptr);
    virtual ~GeneratorPage();

private:
    QScopedPointer<QVBoxLayout> m_Layout;
    QScopedPointer<QProgressBar> m_Progress;
    QScopedPointer<ErrorDlg> m_LogWidget;

    QScopedPointer<ErrorsModel> m_Log;
};

#endif // GENERATORPAGE_H
