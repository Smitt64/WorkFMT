#include "buildtrigwizard.h"

#include <QApplication>
#include <QIcon>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    a.setWindowIcon(QIcon(QStringLiteral(":/res/buildtrig.svg")));

    BuildTrigWizard w;
    w.show();
    return QCoreApplication::exec();
}