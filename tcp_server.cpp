#include "tcp_server.h"
#include "clienthandler.h"

TcpServer::TcpServer(QObject *parent) : QTcpServer(parent)
{
}

void TcpServer::startServer()
{
    if(!listen(QHostAddress::Any, 12345)) {
        qDebug() << "Server could not start!";
    } else {
        qDebug() << "Server started!";
    }
}

void TcpServer::incomingConnection(qintptr socketDescriptor)
{
    ClientHandler *handler = new ClientHandler(socketDescriptor, this);
    connect(handler, &ClientHandler::finished, handler, &ClientHandler::deleteLater);
}
