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

private slots:
    void readData();
    void disconnected();

private:
    void processRequest(const QJsonObject &request);
    bool m_registered = false; // добавить в private

    void handleAuth(const QJsonObject &request);
    void handleRegistration(const QJsonObject &request);
    void controlAC(const QJsonObject &settings);
    void controlMusic(const QJsonObject &settings);
    void controlLight(const QJsonObject &settings);
    void controlKettle(const QJsonObject &settings);
    void controlSocket(const QJsonObject &settings);
    void controlCurtain(const QJsonObject &settings);

    // Если нужен этот метод:
    void controlLightAndKettle();

    void sendResponse(const QJsonObject &response);

    bool m_authenticated;
    QString m_currentUser;
    QTcpSocket *m_socket;
    bool m_acOn = false;
    bool m_lightOn = false;
    bool m_kettleOn = false;
    bool m_socketOn = false;
    bool m_curtainOpen = false;
    bool m_musicOn = false;         // Добавлено: музыка включена/выключена
    QString m_musicTrack;           // Добавлено: текущий трек
    int m_musicVolume = 0;
    void getStatus();
    QString m_acMode;         // режим кондиционера (например, "cool", "heat", "off")
    int m_acTemperature = 0;  // установленная температура кондиционера
};

#endif // CLIENTHANDLER_H
