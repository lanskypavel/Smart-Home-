#pragma once

#include <QObject>
#include <QTcpSocket>
#include <QJsonObject>

class SmartHomeClient : public QObject
{
    Q_OBJECT
public:
    explicit SmartHomeClient(QObject *parent = nullptr);

    void connectToServer(const QString &host, quint16 port);
    void login(const QString &login, const QString &password);
    void registerUser(const QString &login, const QString &password, const QString &email);

    void sendCommand(const QJsonObject &cmd);

signals:
    void serverMessage(const QJsonObject &msg);
    void connected();
    void disconnected();

private slots:
    void onReadyRead();
    void onConnected();
    void onDisconnected();

private:
    QTcpSocket *socket;
    QByteArray buffer;
};
