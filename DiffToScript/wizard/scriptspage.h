#ifndef SCRIPTSPAGE_H
#define SCRIPTSPAGE_H

#include <QWizardPage>
#include <QRunnable>

namespace Ui {
class ScriptsPage;
}

class CodeEditor;
class DiffWizard;
class QSplitter;
class Highlighter;
class QTabWidget;
class ScriptsPage : public QWizardPage
{
    Q_OBJECT

public:
    explicit ScriptsPage(QWidget *parent = nullptr);
    ~ScriptsPage();

    virtual void initializePage() Q_DECL_OVERRIDE;

public slots:
    void oracleScriptReady(const QString &data);
    void postgresScriptReady(const QString &data);

private:
    Ui::ScriptsPage *ui;

    //QSplitter *m_pSplitter;
    QTabWidget *m_pTabWidget;
    CodeEditor *m_pOracle, *m_pPostgres;

    Highlighter *m_pOracleHighlighter, *m_pPostgresHighlighter;
};

class GenerateOperation : public QObject, public QRunnable
{
    Q_OBJECT
public:
    GenerateOperation(DiffWizard *wizrd);
    virtual ~GenerateOperation();

    virtual void run() Q_DECL_OVERRIDE;

signals:
    void oracleScriptReady(const QString &data);
    void postgresScriptReady(const QString &data);

private:
    DiffWizard *m_pWzrd;
};

#endif // SCRIPTSPAGE_H
