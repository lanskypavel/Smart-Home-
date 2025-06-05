#ifndef TCP_SERVER_H
#define TCP_SERVER_H

#include <QTcpServer>
#include <QObject>

class TcpServer : public QTcpServer
{
    Q_OBJECT
public:
    explicit TcpServer(QObject *parent = nullptr);
    bool startServer(); // изменено с void на bool

protected:
    void incomingConnection(qintptr socketDescriptor) override;
};

#endif // TCP_SERVER_H
