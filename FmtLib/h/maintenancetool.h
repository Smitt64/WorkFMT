#ifndef MAINTENANCETOOL_H
#define MAINTENANCETOOL_H

#include <QObject>
#include <QRunnable>
#include <QAtomicInt>

typedef struct
{
    QString id;
    QString name;
    QString version;
    QString sizeString;
    quint64 size;
}CheckUpdateData;

typedef QList<CheckUpdateData> CheckDataList;
class MaintenanceTool : public QObject, public QRunnable
{
    Q_OBJECT
public:
    explicit MaintenanceTool(QObject *parent = nullptr);
    virtual ~MaintenanceTool();

    void run() Q_DECL_OVERRIDE;

    void requestInterruption();
    void setCheckUpdateFlag(bool value);

signals:
    void checkFinished(bool hasUpdates, const CheckDataList &updatedata = CheckDataList());
    void checkStarted();

private:
    QAtomicInt m_Interrupt, m_CheckUpdate;
    QString m_Program;
};

Q_DECLARE_METATYPE(CheckUpdateData)
Q_DECLARE_METATYPE(CheckDataList)

#endif // MAINTENANCETOOL_H
