#ifndef MASSOPBTRVTEMPLATERESULTPAGE_H
#define MASSOPBTRVTEMPLATERESULTPAGE_H

#include <QWizardPage>
#include <QRunnable>
#include <QMap>

#define BFS "*bfs.h"
#define BF "*bf.h"
#define BFCPP "bf_.cpp"
#define SKFCPP "skf_.cpp"
#define FINDCPP "f_.cpp"

namespace Ui {
class MassOpBtrvTemplateResultPage;
}

typedef QMap<QString, QSharedPointer<QByteArray>> GenDataMap;
Q_DECLARE_METATYPE(GenDataMap)

class MassOpBtrvTemplate;
class MassOpBtrvTemplateResultPageRun Q_DECL_FINAL: public QObject, public QRunnable
{
    Q_OBJECT

public:
    MassOpBtrvTemplateResultPageRun(MassOpBtrvTemplate *Interface, QObject *parent = Q_NULLPTR);
    virtual ~MassOpBtrvTemplateResultPageRun() Q_DECL_OVERRIDE;

    void run() Q_DECL_OVERRIDE;

    void setTables(const QStringList &list);

signals:
    void finished(GenDataMap);
    void progress(int);
    void error(QString);
    void message(QString);

private:
    QStringList m_Tables;
    MassOpBtrvTemplate *pInterface;
    GenDataMap m_Data;
};

class ErrorDlg;
class FmtErrors;
class Highlighter;
class MassOpBtrvTemplateResultPage : public QWizardPage
{
    Q_OBJECT

public:
    explicit MassOpBtrvTemplateResultPage(QWidget *parent = nullptr);
    ~MassOpBtrvTemplateResultPage();

    void initializePage() Q_DECL_OVERRIDE;

private slots:
    void finished(const GenDataMap &data);

private:
    void addPage(QWidget *widget, const QString &title);
    void addPage(const QString &title, const QString &data);
    Ui::MassOpBtrvTemplateResultPage *ui;
    QList<Highlighter*> m_Highlighter;
    FmtErrors *pErrors;
    ErrorDlg *pErrDlg;
    bool fIsComplete;
};

#endif // MASSOPBTRVTEMPLATERESULTPAGE_H
