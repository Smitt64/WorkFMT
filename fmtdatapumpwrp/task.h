#ifndef TASK_H
#define TASK_H

#include <QDir>
#include <QObject>
#include <QProcess>

class Task : public QObject
{
    Q_OBJECT
public:
    explicit Task(int argc, char *argv[], QObject *parent = nullptr);

public slots:
    void run();
    void onTimeOut();

signals:
    void finished();

private:
    QString m_Service;
    QString m_SystemName;
    QString m_SystemPass;
    QString m_Directory;
    QString m_OraHome;
    QDir m_DumpDir;
    QString m_LogFile;
    QString m_Action;

    qint64 m_Offset;
    QStringList m_Params;

    QScopedPointer<QProcess> m_Process;
    QScopedPointer<QTimer> m_Timer;
};

#endif // TASK_H
