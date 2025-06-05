#include <QCoreApplication>
#include <QDebug>
#include "tcp_server.h"
#include "DatabaseSingleton.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    qDebug() << "MAIN: 1";
    DatabaseSingleton::instance().initializeDatabase();
    qDebug() << "MAIN: 2";

    TcpServer server;
    qDebug() << "MAIN: 3";

    if (!server.startServer()) {
        qCritical() << "Server could not start!";
        return 1;
    }
    qDebug() << "MAIN: 4";

    return a.exec();
}
