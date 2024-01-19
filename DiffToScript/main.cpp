#include "mainwindow.h"
#include "task.h"
#include "fmtapplication.h"

//#include <QApplication>
#include <QTimer>

int main(int argc, char *argv[])
{
    FmtApplication app2(argc, argv);

    Task *pTask = new Task();
    QObject::connect(pTask, SIGNAL(finished()), &app2, SLOT(quit()));
    QTimer::singleShot(0, pTask, SLOT(run()));

    return app2.exec();
}
