#include "hotfixwizard.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    HotfixWizard wizard;
    wizard.show();

    return a.exec();
}
