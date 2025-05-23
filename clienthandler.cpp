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
    else {
        if(type == "control_ac") {
            controlAC(request["settings"].toObject());
        }
        else if(type == "control_music") {
            controlMusic(request["settings"].toObject());
        }
        else if(type == "control_light_kettle") {
            controlLightAndKettle();
        }
        else {
            sendResponse({{"status", "error"}, {"message", "Unknown command"}});
        }
    }
}

void ClientHandler::handleAuth(const QJsonObject &request)
{
    QString user = request["username"].toString();
    QString pass = request["password"].toString();

    if(DatabaseSingleton::instance().authenticateUser(user, pass)) {
        m_authenticated = true;
        m_currentUser = user;
        sendResponse({{"status", "success"}, {"message", "Authenticated"}});
    }
    else {
        sendResponse({{"status", "error"}, {"message", "Invalid credentials"}});
    }
}

void ClientHandler::handleRegistration(const QJsonObject &request)
{
    QString user = request["username"].toString();
    QString pass = request["password"].toString();

    if(DatabaseSingleton::instance().registerUser(user, pass)) {
        sendResponse({{"status", "success"}, {"message", "User registered"}});
    }
    else {
        sendResponse({{"status", "error"}, {"message", "Registration failed"}});
    }
}

void ClientHandler::controlAC(const QJsonObject &settings)
{
    qInfo() << "AC control:" << settings;
    sendResponse({{"status", "success"}, {"message", "AC settings updated"}});
}

void ClientHandler::controlMusic(const QJsonObject &settings)
{
    qInfo() << "Music control:" << settings;
    sendResponse({{"status", "success"}, {"message", "Music settings updated"}});
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
