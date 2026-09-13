// FieldSplitterProcess.h
#ifndef FIELDSPLITTERPROCESS_H
#define FIELDSPLITTERPROCESS_H

#include <QObject>
#include <QProcess>
#include <QStringList>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QElapsedTimer>
#include <QTimer>
#include <QEventLoop>
#include <QMap>

class FieldSplitterProcess : public QObject
{
    Q_OBJECT

public:
    explicit FieldSplitterProcess(QObject *parent = nullptr);
    ~FieldSplitterProcess();

    // Запуск процесса (неблокирующий)
    bool start(const QString &executablePath = QString());

    // Остановка процесса
    void stop();

    // Проверка, запущен ли процесс
    bool isRunning() const;

    // Получение статуса процесса
    QString statusText() const;
    bool isReady() const; // Готов к приему запросов

    // Синхронная отправка списка полей и получение результатов
    QStringList splitFields(const QStringList &fields, int timeoutMs = 30000);

    // Асинхронная отправка (сигнал resultsReady)
    void splitFieldsAsync(const QStringList &fields, int requestId = -1);

signals:
    void processStarted();
    void processError(const QString &error);
    void processFinished(int exitCode);
    void statusChanged(bool ready);
    void resultsReady(int requestId, const QStringList &results);
    void errorOccurred(int requestId, const QString &error);

private slots:
    void onProcessError(QProcess::ProcessError error);
    void onProcessFinished(int exitCode, QProcess::ExitStatus exitStatus);
    void onReadyRead();
    void checkProcessReady();

private:
    struct PendingRequest {
        QStringList fields;
        int requestId;
        bool waiting;
        QStringList results;
        QString error;
        bool completed;
        QEventLoop loop; // Встроенный event loop для ожидания
    };

    QProcess *m_process;
    QMap<int, PendingRequest*> m_pendingRequests;
    int m_nextRequestId;
    QString m_buffer;
    bool m_processStarted;
    bool m_processReady;
    QTimer *m_readyCheckTimer;
    bool m_debugMode;
    bool m_autoRestart;

    QString sendRequest(const QJsonObject &request);
    PendingRequest* createPendingRequest(const QStringList &fields, int requestId);
    void cleanupPendingRequest(int requestId);
    void updateStatus(bool ready);
    bool isDebugMessage(const QString &line);
};

FieldSplitterProcess *fieldSplitterProcessInstance();

#endif // FIELDSPLITTERPROCESS_H
