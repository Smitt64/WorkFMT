#ifndef RICHTEXTTOINSERTRESULTPAGE_H
#define RICHTEXTTOINSERTRESULTPAGE_H

#include <QWizardPage>

class QLabel;
class CodeEditor;
class RichTextToInsertRun;
class RichTextToInsertResultPage : public QWizardPage
{
    Q_OBJECT
public:
    RichTextToInsertResultPage(QWidget *parent = nullptr);
    virtual ~RichTextToInsertResultPage();

    void initializePage() override;
    bool isComplete() const override { return !m_PlsqlCode.isEmpty(); }

private slots:
    void onCopyToClipboard();
    void onPlsqlGenerated(const QString &plsqlCode);
    void onGenerationFinished();

private:
    void setupUI();
    void generatePlsqlCode();

    CodeEditor *m_pTextEdit;
    QLabel *m_pStatusLabel;
    QPushButton *m_pCopyButton;
    QString m_PlsqlCode;
    RichTextToInsertRun *m_pWorker;
};

#endif // RICHTEXTTOINSERTRESULTPAGE_H
