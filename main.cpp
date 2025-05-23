#include <QCoreApplication>
#include "tcp_server.h"
#include "DatabaseSingleton.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    DatabaseSingleton::instance().initializeDatabase();

    TcpServer server;
    server.startServer(); // Вызов метода из класса

    return a.exec();
}
