#ifndef TASK_H
#define TASK_H

#include <QObject>

class Task : public QObject
{
    Q_OBJECT
public:
    explicit Task(QObject *parent = nullptr);

public slots:
    void run();

signals:
    void finished();

};

#endif // TASK_H
