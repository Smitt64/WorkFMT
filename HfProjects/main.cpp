#include <QApplication>
#include "hfprojectswizzard.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    HfProjectsWizzard w;
    w.show();
    return a.exec();
}
