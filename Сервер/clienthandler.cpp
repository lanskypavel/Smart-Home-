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
    qDebug() << "Получено от клиента:" << data; // Сразу выводим весь сырой пакет

    QList<QByteArray> lines = data.split('\n');
    for (const QByteArray &line : lines) {
        if (line.trimmed().isEmpty())
            continue;
        QJsonDocument doc = QJsonDocument::fromJson(line);
        if (doc.isNull()) {
            qDebug() << "Ошибка парсинга строки:" << line;
            sendResponse({{"status", "error"}, {"message", "Invalid JSON"}});
            continue;
        }
        qDebug() << "JSON получен:" << doc.object();
        processRequest(doc.object());
    }
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
    if (m_authenticated) return;
    QString user = request["username"].toString();
    if (user.isEmpty())
        user = request["login"].toString(); // <-- чтобы работало и так, и так

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
    if (m_registered) return;

    if(DatabaseSingleton::instance().registerUser(user, pass)) {
        sendResponse({{"type", "registration_result"}, {"ok", true}});
    } else {
        sendResponse({{"type", "registration_result"}, {"ok", false}, {"reason", "Registration failed"}});
    }
    if (m_registered) return;
    m_registered = true;

}

void ClientHandler::controlAC(const QJsonObject &settings)
{
    if(settings.contains("on")) {
        m_acOn = settings["on"].toBool();
    }
    if(settings.contains("mode")) {
        m_acMode = settings["mode"].toString();
    }
    if(settings.contains("temp") || settings.contains("temperature")) {
        // поддержка обоих вариантов
        if(settings.contains("temperature"))
            m_acTemperature = settings["temperature"].toInt();
        else
            m_acTemperature = settings["temp"].toInt();
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
    QByteArray data = doc.toJson(QJsonDocument::Compact) + '\n';
    m_socket->write(data);
    m_socket->flush();
    qDebug() << "Ответ отправлен клиенту:" << data;
}

void ClientHandler::disconnected()
{
    emit finished();
    deleteLater();
}
void ClientHandler::controlMusic(const QJsonObject &settings)
{
    qDebug() << "PRE: m_musicOn=" << m_musicOn << "m_musicTrack=" << m_musicTrack << "m_musicVolume=" << m_musicVolume;
    if(settings.contains("on")) {
        m_musicOn = settings["on"].toBool();
    }
    if(settings.contains("track")) {
        m_musicTrack = settings["track"].toString();
    }
    if(settings.contains("volume")) {
        m_musicVolume = settings["volume"].toInt();
    }
    qDebug() << "POST: m_musicOn=" << m_musicOn << "m_musicTrack=" << m_musicTrack << "m_musicVolume=" << m_musicVolume;
    getStatus();
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
        {"ac_on", m_acOn},
        {"ac_mode", m_acMode},
        {"ac_temperature", m_acTemperature}
    };
    sendResponse({{"type", "status"}, {"ok", true}, {"devices", status}});
}
