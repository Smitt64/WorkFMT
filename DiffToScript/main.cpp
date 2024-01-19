#include "mainwindow.h"
#include "task.h"
#include "fmtapplication.h"

//#include <QApplication>
#include <QTimer>

int main(int argc, char *argv[])
{
//    QApplication a(argc, argv);

    //QScopedPointer<QCoreApplication> app1(new QApplication(argc, argv));
    FmtApplication app2(argc, argv);

    Task *pTask = new Task();
    QObject::connect(pTask, SIGNAL(finished()), &app2, SLOT(quit()));
    QTimer::singleShot(0, pTask, SLOT(run()));

    return app2.exec();


    //emit dp.finished();
//    MainWindow w;
//    w.show();

    //return a.exec();
    return 0;
}
