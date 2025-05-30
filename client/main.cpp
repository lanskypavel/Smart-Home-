#include "managerforwindows.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    ManagerForWindows manager;
    manager.showAuthWindow();

    return a.exec();
}
