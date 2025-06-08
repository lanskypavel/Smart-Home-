#include "tcp_server.h"
#include "clienthandler.h"
#include <QDebug>

TcpServer::TcpServer(QObject *parent) : QTcpServer(parent)
{
}

bool TcpServer::startServer()
{
    if(!listen(QHostAddress("127.0.0.1"), 12345)) {
        qDebug() << "Server could not start! Reason:" << errorString();
        return false;
    } else {
        qDebug() << "Server started!";
        return true;
    }
}

void TcpServer::incomingConnection(qintptr socketDescriptor)
{
    ClientHandler *handler = new ClientHandler(socketDescriptor, this);
    connect(handler, &ClientHandler::finished, handler, &QObject::deleteLater);
}
