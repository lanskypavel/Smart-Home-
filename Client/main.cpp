#include "smarthomewindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    qDebug() << "main.cpp стартует!";
    QApplication a(argc, argv);
    SmarthomeWindow w;
    w.show();
    return a.exec();
}
