#include "smart_home_client.h"
#include <QJsonDocument>

SmartHomeClient::SmartHomeClient(QObject *parent) : QObject(parent)
{
    socket = new QTcpSocket(this);
    connect(socket, &QTcpSocket::readyRead, this, &SmartHomeClient::onReadyRead);
    connect(socket, &QTcpSocket::connected, this, &SmartHomeClient::onConnected);
    connect(socket, &QTcpSocket::disconnected, this, &SmartHomeClient::onDisconnected);
}

void SmartHomeClient::connectToServer(const QString &host, quint16 port)
{
    qDebug() << "Пытаюсь подключиться к серверу...";
    socket->connectToHost(host, port);
    connect(socket, &QTcpSocket::connected, this, [](){ qDebug() << "СОЕДИНЕНИЕ УСТАНОВЛЕНО"; });
    connect(socket, &QTcpSocket::errorOccurred, this, [](QAbstractSocket::SocketError err){ qDebug() << "Ошибка сокета:" << err; });}

void SmartHomeClient::login(const QString &login, const QString &password)
{
    QJsonObject obj;
    obj["type"] = "auth";
    obj["username"] = login;
    obj["password"] = password;
    sendCommand(obj);
}

void SmartHomeClient::registerUser(const QString &login, const QString &password, const QString &email)
{
    QJsonObject obj;
    obj["type"] = "registration";
    obj["username"] = login;
    obj["password"] = password;
    obj["email"] = email;
    sendCommand(obj);
}
void SmartHomeClient::sendCommand(const QJsonObject &cmd)
{
    QJsonDocument doc(cmd);
    QByteArray data = doc.toJson(QJsonDocument::Compact) + '\n';
    qDebug() << data;
    socket->write(data);
}

void SmartHomeClient::onReadyRead()
{
    qDebug() << "onReadyRead: получены данные от сервера";
    buffer.append(socket->readAll());
    while (true) {
        int idx = buffer.indexOf('\n');
        if (idx < 0) break;
        QByteArray line = buffer.left(idx).trimmed();
        buffer.remove(0, idx + 1);
        qDebug() << "Получена строка от сервера:" << line;
        QJsonParseError err;
        QJsonDocument doc = QJsonDocument::fromJson(line, &err);
        if (err.error == QJsonParseError::NoError && doc.isObject()) {
            qDebug() << "SmartHomeClient: emit serverMessage:" << doc.object();
            emit serverMessage(doc.object());
        } else {
            qDebug() << "Ошибка парсинга JSON:" << err.errorString() << "в строке:" << line;
        }
    }
}

void SmartHomeClient::onConnected()
{
    emit connected();
}

void SmartHomeClient::onDisconnected()
{
    emit disconnected();
}
