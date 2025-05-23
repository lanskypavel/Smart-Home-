#ifndef CLIENTHANDLER_H
#define CLIENTHANDLER_H

#include <QtCore/QObject>
#include <QtNetwork/QTcpSocket>
#include <QtCore/QJsonDocument>
#include <QtCore/QJsonObject>
#include <QtCore/QDateTime>
#include <QtCore/QString>

class ClientHandler : public QObject
{
    Q_OBJECT
public:
    explicit ClientHandler(qintptr socketDescriptor, QObject *parent = nullptr);

signals:
    void finished();

public slots:
    void readData();
    void disconnected();

private:
    void processRequest(const QJsonObject &request);
    void handleAuth(const QJsonObject &request);
    void handleRegistration(const QJsonObject &request);
    void sendResponse(const QJsonObject &response);

    void controlAC(const QJsonObject &settings);
    void controlMusic(const QJsonObject &settings);
    void controlLightAndKettle();

    QTcpSocket *m_socket;
    bool m_authenticated;
    QString m_currentUser;
};

#endif // CLIENTHANDLER_H
