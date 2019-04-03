#ifndef MASSOPENFUCNPARAMS_H
#define MASSOPENFUCNPARAMS_H

#include <QWizardPage>
#include <QScopedPointer>

namespace Ui {
class MassOpenFucnParams;
}

class CodeEditor;
class MassOpenFucnParams : public QWizardPage
{
    Q_OBJECT

public:
    explicit MassOpenFucnParams(QWidget *parent = nullptr);
    ~MassOpenFucnParams();

private:
    Ui::MassOpenFucnParams *ui;
    CodeEditor *pEditor;
};

#endif // MASSOPENFUCNPARAMS_H
