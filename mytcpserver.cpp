// mytcpserver.cpp
#include "mytcpserver.h"
#include <QDebug>
#include <QCoreApplication>
#include <QString>
#include <QHostAddress>

MyTcpServer::~MyTcpServer()
{
    foreach (QTcpSocket *socket, mClients.keys()) {
        socket->close();
        socket->deleteLater();
    }
    mTcpServer->close();
}

MyTcpServer::MyTcpServer(QObject *parent) : QObject(parent){
    mTcpServer = new QTcpServer(this);

    connect(mTcpServer, &QTcpServer::newConnection,
            this, &MyTcpServer::slotNewConnection);

    if(!mTcpServer->listen(QHostAddress::Any, 33333)){
        qDebug() << "server is not started";
    } else {
        qDebug() << "server is started";
    }
}

void MyTcpServer::slotNewConnection(){
    QTcpSocket *clientSocket = mTcpServer->nextPendingConnection();

    // Создаем идентификатор клиента (IP:port)
    QString clientInfo = QString("%1:%2").arg(clientSocket->peerAddress().toString()).arg(clientSocket->peerPort());
    mClients[clientSocket] = clientInfo;

    qDebug() << "New client connected:" << clientInfo;

    clientSocket->write(QString("Hello, %1! I am echo server!\r\n").arg(clientInfo).toUtf8());

    connect(clientSocket, &QTcpSocket::readyRead, this, &MyTcpServer::slotServerRead);
    connect(clientSocket, &QTcpSocket::disconnected, this, &MyTcpServer::slotClientDisconnected);
}

void MyTcpServer::slotServerRead(){
    QTcpSocket *clientSocket = qobject_cast<QTcpSocket*>(sender());
    if (!clientSocket || !mClients.contains(clientSocket)) return;

    QString clientInfo = mClients[clientSocket];
    QString res = "";

    while(clientSocket->bytesAvailable()>0)
    {
        QByteArray array = clientSocket->readAll();
        qDebug() << "From" << clientInfo << ":" << array << "\n";

        if(array=="\x01")
        {
            clientSocket->write(res.toUtf8());
            res = "";
        }
        else
            res.append(array);
    }

    if (!res.isEmpty()) {
        clientSocket->write(res.toUtf8());
    }
}

void MyTcpServer::slotClientDisconnected(){
    QTcpSocket *clientSocket = qobject_cast<QTcpSocket*>(sender());
    if (!clientSocket) return;

    qDebug() << "Client disconnected:" << mClients[clientSocket];
    mClients.remove(clientSocket);
    clientSocket->deleteLater();
}
