#ifndef OPERATIONBASE_H
#define OPERATIONBASE_H

#include <QObject>
#include <QRunnable>

class RegParmWizard;
class OperationBase : public QObject, public QRunnable
{
    Q_OBJECT
public:
    explicit OperationBase(QObject *parent = nullptr);
    ~OperationBase() = default;

    void setWizzard(RegParmWizard *wzrd);
    RegParmWizard *wizzard();

signals:
    void finished();
    void errorMessage(QString);
    void warningMessage(QString);
    void informationMessage(QString);

private:
    RegParmWizard *m_pWizzard;

};

#endif // OPERATIONBASE_H
