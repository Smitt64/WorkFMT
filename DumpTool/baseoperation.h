#ifndef BASEOPERATION_H
#define BASEOPERATION_H

#include "dumptoolwizard.h"
#include <QRunnable>
#include <QObject>

class QTextDocument;
class BaseLogHighlighter;
class BaseOperation : public QObject, public QRunnable
{
    Q_OBJECT
public:
    BaseOperation(DumpToolWizard *wizzard);
    virtual ~BaseOperation();

    DumpToolWizard *wizzard();
    virtual void highlighter(BaseLogHighlighter **obj, QTextDocument *parent);

Q_SIGNALS:
    void message(QString);
    void errormsg(QString);
    void succeeded(QString);
    void finished();
    void progressDialog(QString, qint64);
    void progressValue(int);
    void progressDialogEnd();
    void stageChanged(QString);

protected:
    bool UnpackFile(const QString &res, const QString &dst);

private:
    DumpToolWizard *m_pWizzard;
};

#endif // BASEOPERATION_H
