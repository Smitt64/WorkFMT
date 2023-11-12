#include <QCoreApplication>
#include <QTextStream>
#include <QProcess>
#include <fmtcore.h>
#include "task.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    QDir current(QDir::current());
    a.addLibraryPath(QFileInfo(QCoreApplication::applicationFilePath()).path());
    a.addLibraryPath(current.absolutePath());
    a.addLibraryPath(current.absoluteFilePath("platforms"));
    a.addLibraryPath(current.absoluteFilePath("iconengines"));
    a.addLibraryPath(current.absoluteFilePath("imageformats"));
    a.addLibraryPath(current.absoluteFilePath("platforms"));
    a.addLibraryPath(current.absoluteFilePath("sqldrivers"));
    a.addLibraryPath(current.absoluteFilePath("styles"));

    Task *task = new Task(argc, argv);
    QObject::connect(task, SIGNAL(finished()), &a, SLOT(quit()));
    QTimer::singleShot(100, task, SLOT(run()));

    return a.exec();
}
