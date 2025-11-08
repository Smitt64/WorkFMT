#include <QApplication>
#include <QDir>
#include "regparmwizard.h"

int main(int argc, char *argv[])
{
    QCoreApplication::setOrganizationName("RegParmTool");
    QCoreApplication::setOrganizationDomain("RegParmTool");
    QCoreApplication::setApplicationName("RegParmTool");
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

    RegParmWizard w;
    w.show();

    return app->exec();
}
