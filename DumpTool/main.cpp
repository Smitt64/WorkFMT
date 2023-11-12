#include "dumptoolwizard.h"
#include "loggingcategories.h"
#include <QApplication>
#include <QDir>
#include <QFileInfo>
#include <QTranslator>

int main(int argc, char *argv[])
{
    QTranslator qt_translator;
    QApplication a(argc, argv);

    QDir current(QDir::current());
    a.addLibraryPath(QFileInfo(QCoreApplication::applicationFilePath()).path());
    a.addLibraryPath(current.absolutePath());
    a.addLibraryPath(current.absoluteFilePath("platforms"));
    a.addLibraryPath(current.absoluteFilePath("iconengines"));
    a.addLibraryPath(current.absoluteFilePath("imageformats"));
    a.addLibraryPath(current.absoluteFilePath("platforms"));
    a.addLibraryPath(current.absoluteFilePath("sqldrivers"));
    a.addLibraryPath(current.absoluteFilePath("styles"));

    QDir trDir(QApplication::applicationDirPath());
    if (!trDir.cd("translations"))
        trDir = QDir::current();

    if (trDir.cd("translations"))
    {
        QString translatorFile = QString("qt_%1").arg("ru");
        if (qt_translator.load(translatorFile, trDir.absolutePath()))
        {
            //qCInfo(logCore()) << "Translator installed: " << translatorFile;
            QApplication::installTranslator(&qt_translator);
        }
        /*else
            qCWarning(logCore()) << "Error loading translator " << translatorFile;*/
    }
    /*else
        qCWarning(logCore()) << "Can't find translations folder";*/

    DumpToolWizard wizard;
    wizard.show();
    return a.exec();
}
