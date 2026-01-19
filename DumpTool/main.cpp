#include "dumptoolwizard.h"
#include "loggingcategories.h"
#include <QApplication>
#include <QDir>
#include <QFileInfo>
#include <QTranslator>
#include <QMessageBox>
#include <toolsruntime.h>

int main(int argc, char *argv[])
{
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
    else
        trDir = QApplication::applicationDirPath();

    QTranslator qt_translator;
    if (trDir.cd("translations"))
    {
        QString translatorFile = QString("qt_%1").arg("ru");
        if (qt_translator.load(translatorFile, trDir.absolutePath()))
        {
            //qCInfo(logCore()) << "Translator installed: " << translatorFile;
            QApplication::installTranslator(&qt_translator);
        }
        else
            QMessageBox::critical(nullptr, "Ошибка!", QString("Не удалось загрузить перевод %1").arg(translatorFile));
    }

    DumpToolWizard wizard;
    wizard.show();
    return a.exec();
}
