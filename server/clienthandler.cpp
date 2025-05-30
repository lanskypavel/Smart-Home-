#include "clienthandler.h"
#include "DatabaseSingleton.h"
#include <QtCore/QDebug>

ClientHandler::ClientHandler(qintptr socketDescriptor, QObject *parent)
    : QObject(parent), m_authenticated(false), m_socket(nullptr)
{
    m_socket = new QTcpSocket(this);
    connect(m_socket, &QTcpSocket::readyRead, this, &ClientHandler::readData);
    connect(m_socket, &QTcpSocket::disconnected, this, &ClientHandler::disconnected);

    if(!m_socket->setSocketDescriptor(socketDescriptor)) {
        qWarning() << "Error setting socket descriptor";
        deleteLater();
    }
}

void ClientHandler::readData()
{
    QByteArray data = m_socket->readAll();
    QJsonDocument doc = QJsonDocument::fromJson(data);

    if(doc.isNull()) {
        sendResponse({{"status", "error"}, {"message", "Invalid JSON"}});
        return;
    }

    processRequest(doc.object());
}

void ClientHandler::processRequest(const QJsonObject &request)
{
    QString type = request["type"].toString();

    if(type == "auth") {
        handleAuth(request);
    }
    else if(type == "registration") {
        handleRegistration(request);
    }
    else if(!m_authenticated) {
        sendResponse({{"status", "error"}, {"message", "Not authenticated"}});
    }
    else if(type == "control_ac") {
        controlAC(request["settings"].toObject());
    }
    else if(type == "control_music") {
        controlMusic(request["settings"].toObject());
    }
    else if(type == "control_light") {
        controlLight(request["settings"].toObject());
    }
    else if(type == "control_kettle") {
        controlKettle(request["settings"].toObject());
    }
    else if(type == "control_socket") {
        controlSocket(request["settings"].toObject());
    }
    else if(type == "control_curtain") {
        controlCurtain(request["settings"].toObject());
    }
    else if(type == "control_light_kettle") {
        controlLightAndKettle();
    }
    else if(type == "get_status") {
        getStatus();
    }
    else {
        sendResponse({{"status", "error"}, {"message", "Unknown command"}});
    }
}

void ClientHandler::handleAuth(const QJsonObject &request)
{
    QString user = request["username"].toString();
    QString pass = request["password"].toString();

    if(DatabaseSingleton::instance().authenticateUser(user, pass)) {
        m_authenticated = true;
        m_currentUser = user;
        sendResponse({{"type", "auth_result"}, {"ok", true}});
    } else {
        sendResponse({{"type", "auth_result"}, {"ok", false}, {"reason", "Invalid credentials"}});
    }
}

void ClientHandler::handleRegistration(const QJsonObject &request)
{
    QString user = request["username"].toString();
    QString pass = request["password"].toString();

    if(DatabaseSingleton::instance().registerUser(user, pass)) {
        sendResponse({{"type", "registration_result"}, {"ok", true}});
    } else {
        sendResponse({{"type", "registration_result"}, {"ok", false}, {"reason", "Registration failed"}});
    }
}

void ClientHandler::controlAC(const QJsonObject &settings)
{
    if(settings.contains("mode")) {
        m_acMode = settings["mode"].toString();
    }
    if(settings.contains("temperature")) {
        m_acTemperature = settings["temperature"].toInt();
    }
    qInfo() << "AC control:" << settings;
    sendResponse({{"status", "success"}, {"message", "AC settings updated"}});
}

void ClientHandler::controlLight(const QJsonObject &settings)
{
    if(settings.contains("on")) {
        m_lightOn = settings["on"].toBool();
        sendResponse({{"status", "success"}, {"message", m_lightOn ? "Light turned on" : "Light turned off"}});
    }
    // Можно добавить обработку расписания
}

void ClientHandler::controlKettle(const QJsonObject &settings)
{
    if(settings.contains("on")) {
        m_kettleOn = settings["on"].toBool();
        sendResponse({{"status", "success"}, {"message", m_kettleOn ? "Kettle turned on" : "Kettle turned off"}});
    }
    // Можно добавить обработку расписания
}

void ClientHandler::controlSocket(const QJsonObject &settings)
{
    if(settings.contains("on")) {
        m_socketOn = settings["on"].toBool();
        sendResponse({{"status", "success"}, {"message", m_socketOn ? "Socket enabled" : "Socket disabled"}});
    }
}

void ClientHandler::controlCurtain(const QJsonObject &settings)
{
    QString action = settings["action"].toString();
    if(action == "open") {
        m_curtainOpen = true;
        sendResponse({{"status", "success"}, {"message", "Curtain opened"}});
    } else if(action == "close") {
        m_curtainOpen = false;
        sendResponse({{"status", "success"}, {"message", "Curtain closed"}});
    } else {
        sendResponse({{"status", "error"}, {"message", "Unknown curtain action"}});
    }
}

void ClientHandler::controlLightAndKettle()
{
    QDateTime now = QDateTime::currentDateTime();
    bool lightsOn = now.time().hour() >= 18 || now.time().hour() <= 6;
    bool kettleOn = now.time().hour() >= 7 && now.time().hour() <= 9;

    QJsonObject response {
        {"status", "success"},
        {"lights", lightsOn},
        {"kettle", kettleOn}
    };
    sendResponse(response);
}

void ClientHandler::sendResponse(const QJsonObject &response)
{
    QJsonDocument doc(response);
    m_socket->write(doc.toJson());
}

void ClientHandler::disconnected()
{
    emit finished();
    deleteLater();
}
void ClientHandler::controlMusic(const QJsonObject &settings)
{
    if(settings.contains("on")) {
        m_musicOn = settings["on"].toBool();
    }
    if(settings.contains("track")) {
        m_musicTrack = settings["track"].toString();
    }
    if(settings.contains("volume")) {
        m_musicVolume = settings["volume"].toInt();
    }
    qInfo() << "Music control:" << settings;
    sendResponse({{"status", "success"}, {"message", "Music settings updated"}});
}
void ClientHandler::getStatus()
{
    QJsonObject status {
        {"light", m_lightOn},
        {"kettle", m_kettleOn},
        {"socket", m_socketOn},
        {"curtain", m_curtainOpen},
        {"music_on", m_musicOn},
        {"music_track", m_musicTrack},
        {"music_volume", m_musicVolume},
        {"ac_mode", m_acMode},
        {"ac_temperature", m_acTemperature}
    };
    sendResponse({{"type", "status"}, {"ok", true}, {"devices", status}});
}
