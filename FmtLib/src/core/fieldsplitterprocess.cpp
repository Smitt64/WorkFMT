// FieldSplitterProcess.cpp
#include "FieldSplitterProcess.h"
#include "loggingcategories.h"
#include <QCoreApplication>
#include <QDebug>
#include <QThread>
#include <QDir>
#include <QRegularExpression>
#include "fmtcore.h"
#include "toolsruntime.h"
#include <QFileInfo>

Q_GLOBAL_STATIC(FieldSplitterProcess, _fieldSplitterProcessInstance)

FieldSplitterProcess *fieldSplitterProcessInstance()
{
    return _fieldSplitterProcessInstance;
}

namespace
{
const QString PROCESS_STARTED_MSG = "JSON mode started. PID:";
const QString PROCESS_INITIALIZED_MSG = "Splitter initialized successfully";
const QString PROCESSED_REQUEST_MSG = "Processed request";
}

FieldSplitterProcess::FieldSplitterProcess(QObject *parent)
    : QObject(parent)
    , m_process(new QProcess(this))
    , m_nextRequestId(0)
    , m_processStarted(false)
    , m_processReady(false)
    , m_readyCheckTimer(new QTimer(this))
    , m_debugMode(false)
    , m_autoRestart(false)
{
    connect(m_process, &QProcess::readyReadStandardError, [this]()
    {
        QString data = QString::fromUtf8(m_process->readAllStandardError());
        if (m_debugMode && !data.isEmpty())
            qCInfo(logCore()) << "Python stderr:" << data.trimmed();
    });

    m_process->setProcessChannelMode(QProcess::SeparateChannels);

    m_readyCheckTimer->setInterval(500);
    connect(m_readyCheckTimer, &QTimer::timeout, this, &FieldSplitterProcess::checkProcessReady);
    connect(m_process, &QProcess::errorOccurred, this, &FieldSplitterProcess::onProcessError);
    connect(m_process, QOverload<int, QProcess::ExitStatus>::of(&QProcess::finished), this, &FieldSplitterProcess::onProcessFinished);
    connect(m_process, &QProcess::readyReadStandardOutput, this, &FieldSplitterProcess::onReadyRead);
}

FieldSplitterProcess::~FieldSplitterProcess()
{
    stop();
    qDeleteAll(m_pendingRequests);
    m_pendingRequests.clear();
}

bool FieldSplitterProcess::isDebugMessage(const QString &line)
{
    static const QRegularExpression debugPattern("^(JSON mode started\\. PID:|Splitter initialized successfully|Processed request)");
    return line.contains(debugPattern);
}

bool FieldSplitterProcess::start(const QString &executablePath)
{
    if (isRunning())
        return true;

    QString exePath = executablePath;
    if (exePath.isEmpty())
        exePath = toolFullFileNameFromDir("CapitalizeField.exe");

    QFileInfo fi(exePath);
    m_process->setWorkingDirectory(fi.path());

    QStringList arguments;
    arguments << "--json";

    int result = CoreStartProcess(m_process, exePath, arguments, false, false, 0, false);

    if (result == 0)
    {
        m_processStarted = true;
        m_processReady = false;
        m_readyCheckTimer->start();
        updateStatus(false);
        emit processStarted();
        qCInfo(logCore()) << "FieldSplitter process started with PID:" << m_process->processId();
    }
    else
    {
        m_processStarted = false;
        qCInfo(logCore()) << "Failed to start FieldSplitter process";
        updateStatus(false);
    }

    return m_processStarted;
}

void FieldSplitterProcess::stop()
{
    m_readyCheckTimer->stop();

    if (m_process && m_process->state() != QProcess::NotRunning)
    {
        qCInfo(logCore()) << "Stopping FieldSplitter process gracefully";

        QJsonObject exitCommand;
        exitCommand["command"] = "exit";
        QJsonDocument doc(exitCommand);
        m_process->write(doc.toJson(QJsonDocument::Compact) + '\n');
        m_process->waitForBytesWritten(1000);

        if (!m_process->waitForFinished(3000))
        {
            m_process->terminate();
            if (!m_process->waitForFinished(2000))
                m_process->kill();
        }
    }

    m_processStarted = false;
    m_processReady = false;
    updateStatus(false);
}

bool FieldSplitterProcess::isRunning() const
{
    return m_processStarted && m_process && m_process->state() != QProcess::NotRunning;
}

bool FieldSplitterProcess::isReady() const
{
    if (m_processStarted && !m_processReady)
    {
        FieldSplitterProcess *pThis = const_cast<FieldSplitterProcess*>(this);
        pThis->checkProcessReady();
    }

    return m_processStarted && m_processReady;
}

QString FieldSplitterProcess::statusText() const
{
    if (!m_processStarted)
        return "Процесс не запущен";

    if (!m_processReady)
        return "Инициализация...";

    return "Готов к работе";
}

void FieldSplitterProcess::checkProcessReady()
{
    if (!isRunning())
    {
        m_processReady = false;
        updateStatus(false);
        return;
    }

    static bool checking = false;
    if (checking)
        return;

    checking = true;

    QJsonObject request;
    request["requestId"] = -999;
    request["field"] = "test";

    QString error = sendRequest(request);
    if (error.isEmpty())
    {
        if (!m_processReady)
        {
            m_processReady = true;
            updateStatus(true);
            qCInfo(logCore()) << "FieldSplitter process is ready";
        }
    }

    checking = false;
}

void FieldSplitterProcess::updateStatus(bool ready)
{
    emit statusChanged(ready);
}

QStringList FieldSplitterProcess::splitFields(const QStringList &fields, int timeoutMs)
{
    if (!isReady())
        return QStringList();

    int requestId = m_nextRequestId++;

    PendingRequest *request = createPendingRequest(fields, requestId);
    if (!request)
        return QStringList();

    QJsonObject requestObj;
    requestObj["requestId"] = requestId;
    requestObj["batch"] = true;
    requestObj["field"] = QJsonArray::fromStringList(fields);

    QString error = sendRequest(requestObj);
    if (!error.isEmpty())
    {
        cleanupPendingRequest(requestId);
        qCWarning(logCore()) << "Failed to send request:" << error;
        return QStringList();
    }

    QTimer timer;
    timer.setSingleShot(true);
    connect(&timer, &QTimer::timeout, &request->loop, &QEventLoop::quit);

    timer.start(timeoutMs);
    if (request->waiting && !request->completed)
        request->loop.exec(QEventLoop::ExcludeUserInputEvents);

    timer.stop();

    if (!request->completed)
    {
        qCWarning(logCore()) << "Request timeout after" << timeoutMs << "ms";
        cleanupPendingRequest(requestId);
        return QStringList();
    }

    QStringList results = request->results;
    QString reqError = request->error;
    cleanupPendingRequest(requestId);

    if (!reqError.isEmpty())
    {
        qCWarning(logCore()) << "Request error:" << reqError;
        return QStringList();
    }

    return results;
}

void FieldSplitterProcess::splitFieldsAsync(const QStringList &fields, int requestId)
{
    if (!isReady())
    {
        emit errorOccurred(requestId, "Process not ready");
        return;
    }

    if (requestId < 0)
        requestId = m_nextRequestId++;

    PendingRequest *request = createPendingRequest(fields, requestId);
    if (!request)
    {
        emit errorOccurred(requestId, "Failed to create request");
        return;
    }

    request->waiting = false;

    QJsonObject requestObj;
    requestObj["requestId"] = requestId;
    requestObj["batch"] = true;
    requestObj["field"] = QJsonArray::fromStringList(fields);

    QString error = sendRequest(requestObj);
    if (!error.isEmpty())
    {
        cleanupPendingRequest(requestId);
        emit errorOccurred(requestId, error);
    }
}

void FieldSplitterProcess::onProcessError(QProcess::ProcessError error)
{
    qCWarning(logCore()) << "FieldSplitter process error:" << GetProcessErrorText(error);

    m_processStarted = false;
    m_processReady = false;
    m_readyCheckTimer->stop();
    updateStatus(false);

    for (auto *request : qAsConst(m_pendingRequests))
    {
        request->completed = true;
        request->error = "Process error: " + GetProcessErrorText(error);
        if (request->waiting)
            request->loop.quit();
    }

    emit processError(GetProcessErrorText(error));
}

void FieldSplitterProcess::onProcessFinished(int exitCode, QProcess::ExitStatus exitStatus)
{
    qCInfo(logCore()) << "FieldSplitter process finished. Exit code:" << exitCode
                      << "Status:" << ProcessExitStatusText(exitStatus);

    if (exitCode == 0 && exitStatus == QProcess::NormalExit)
        qCInfo(logCore()) << "Python process exited normally";
    else
        qCWarning(logCore()) << "Python process exited abnormally";

    m_processStarted = false;
    m_processReady = false;
    m_readyCheckTimer->stop();
    updateStatus(false);

    for (auto *request : m_pendingRequests)
    {
        request->completed = true;
        request->error = "Process finished unexpectedly";

        if (request->waiting)
            request->loop.quit();
    }

    emit processFinished(exitCode);

    if (m_autoRestart)
    {
        QTimer::singleShot(5000, this, [this]()
        {
            if (!isRunning())
            {
                qCInfo(logCore()) << "Auto-restarting process...";
                start();
            }
        });
    }
}

void FieldSplitterProcess::onReadyRead()
{
    m_buffer += QString::fromUtf8(m_process->readAllStandardOutput());

    while (m_buffer.contains('\n'))
    {
        int newlinePos = m_buffer.indexOf('\n');
        QString line = m_buffer.left(newlinePos).trimmed();
        m_buffer = m_buffer.mid(newlinePos + 1);

        if (line.isEmpty())
            continue;

        // Пропускаем отладочные сообщения
        if (isDebugMessage(line))
        {
            if (m_debugMode)
                qCInfo(logCore()) << "Python debug:" << line;

            continue;
        }

        QJsonParseError parseError;
        QJsonDocument doc = QJsonDocument::fromJson(line.toUtf8(), &parseError);

        if (parseError.error != QJsonParseError::NoError)
        {
            if (m_debugMode)
            {
                qCWarning(logCore()) << "Failed to parse JSON response:" << parseError.errorString();
                qCWarning(logCore()) << "Raw line:" << line;
            }

            continue;
        }

        QJsonObject response = doc.object();
        int requestId = response["requestId"].toInt(-1);

        if (requestId < 0)
        {
            if (m_debugMode)
                qCWarning(logCore()) << "Response missing requestId";

            continue;
        }

        PendingRequest *request = m_pendingRequests.value(requestId);
        if (!request)
        {
            if (m_debugMode)
                qCWarning(logCore()) << "Received response for unknown request:" << requestId;

            continue;
        }

        // Обрабатываем ответ
        if (response["success"].toBool())
        {
            if (response.contains("batch") && response["batch"].toBool())
            {
                QJsonArray results = response["results"].toArray();
                QStringList formattedResults;

                for (const QJsonValue &val : results)
                {
                    QJsonObject resultObj = val.toObject();
                    if (resultObj["success"].toBool())
                        formattedResults.append(resultObj["formatted"].toString());
                    else
                        formattedResults.append(resultObj["original"].toString() + " (error)");
                }

                request->results = formattedResults;
            }
            else
                request->results = QStringList() << response["formatted"].toString();
        }
        else
            request->error = response["error"].toString();

        request->completed = true;

        if (request->waiting)
            request->loop.quit(); // Выходим из event loop
        else
        {
            emit resultsReady(requestId, request->results);
            cleanupPendingRequest(requestId);
        }
    }
}

QString FieldSplitterProcess::sendRequest(const QJsonObject &request)
{
    if (!isRunning())
        return "Process not running";

    QJsonDocument doc(request);
    QByteArray data = doc.toJson(QJsonDocument::Compact) + '\n';

    qint64 written = m_process->write(data);
    if (written != data.size())
        return "Failed to write all data to process";

    if (!m_process->waitForBytesWritten(5000))
        return "Timeout waiting for bytes to be written";

    return QString();
}

FieldSplitterProcess::PendingRequest* FieldSplitterProcess::createPendingRequest(
    const QStringList &fields, int requestId)
{
    if (m_pendingRequests.contains(requestId)) {
        qCWarning(logCore()) << "Request ID already exists:" << requestId;
        return nullptr;
    }

    PendingRequest *request = new PendingRequest;
    request->fields = fields;
    request->requestId = requestId;
    request->waiting = true;
    request->completed = false;

    m_pendingRequests[requestId] = request;

    return request;
}

void FieldSplitterProcess::cleanupPendingRequest(int requestId)
{
    PendingRequest *request = m_pendingRequests.take(requestId);
    if (request) {
        delete request;
    }
}
