#include "dbmainwindow.h"
#include "dbfileobject.h"
#include <QApplication>
#include "task.h"

int main(int argc, char *argv[])
{
    QCoreApplication::setOrganizationName("DBFileTool");
    QCoreApplication::setOrganizationDomain("DBFileTool");
    QCoreApplication::setApplicationName("DBFileTool");
    QScopedPointer<QCoreApplication> app(new QApplication(argc, argv));

    QDir current(QDir::current());
    app->addLibraryPath(QFileInfo(QCoreApplication::applicationFilePath()).path());
    app->addLibraryPath(current.absolutePath());
    app->addLibraryPath(current.absoluteFilePath("platforms"));
    app->addLibraryPath(current.absoluteFilePath("iconengines"));
    app->addLibraryPath(current.absoluteFilePath("imageformats"));
    app->addLibraryPath(current.absoluteFilePath("platforms"));
    app->addLibraryPath(current.absoluteFilePath("sqldrivers"));
    app->addLibraryPath(current.absoluteFilePath("styles"));

    Task *pTask = new Task(argc, argv, app.data());
    QObject::connect(pTask, SIGNAL(finished()), app.data(), SLOT(quit()));
    QTimer::singleShot(0, pTask, SLOT(run()));

    return app->exec();
}

