#ifndef TASK_H
#define TASK_H

#include <array>

#include <QObject>
#include <QTextStream>
#include <QFile>

#include "taskoptions.h"

class TableLinks;
class QSettings;
class Task : public QObject
{
    Q_OBJECT
public:
    TaskOptions optns;
    explicit Task(QObject *parent = nullptr);
//    void setOption(TaskOption opt, bool val) {optns[opt] = val;}
//    bool getOption(TaskOption opt) const { return optns[opt];}
    void runScriptTask();
    void runDiffTask();
    void runNormalPathsTask();
    void runAlterTableTask();
    void setInput(QString fileName);

    int result() const;

public slots:
    void run();

signals:
    void finished();

private:
    void showAppInfo(QTextStream& os);
    void makeInputBuff(QString& buff, const TaskOptions& optns);
    void makeOutputStream(QTextStream& os, const TaskOptions& optns);

    int m_Result;
};

QSharedPointer<QSettings> diffGetSettings();

extern QStringList GetClearedFiles(const QStringList &files, QList<TableLinks *> &tableLinks);
extern QStringList GetNormalFileList(const QStringList files,
                                     const QList<TableLinks *> &tableLinks,
                                     std::function<void(const QString &file)> userfunc);

extern void generateUpdateScript(const QByteArray &jsonData, QTextStream &stream, int indentSpaces = 4);

#endif // TASK_H
