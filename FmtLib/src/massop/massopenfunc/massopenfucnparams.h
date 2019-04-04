#ifndef MASSOPENFUCNPARAMS_H
#define MASSOPENFUCNPARAMS_H

#include <QWizardPage>
#include <QScopedPointer>
#include <QMap>

namespace Ui {
class MassOpenFucnParams;
}

class CodeEditor;
class Highlighter;
class MassOpenFucnParams : public QWizardPage
{
    Q_OBJECT

public:
    explicit MassOpenFucnParams(QWidget *parent = nullptr);
    ~MassOpenFucnParams();

    virtual void initializePage() Q_DECL_OVERRIDE;

private slots:
    void textChanged(const QString &text);

private:
    Ui::MassOpenFucnParams *ui;
    CodeEditor *pEditor;
    QByteArray m_template;
    QMap<QString, QString> params;
    Highlighter *m_Highlighter;
};

#endif // MASSOPENFUCNPARAMS_H
