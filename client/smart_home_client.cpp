#include "smart_home_client.h"
#include <QDebug>

SmartHomeClient* SmartHomeClient::m_instance = nullptr;

SmartHomeClient::SmartHomeClient(QObject *parent) : QObject(parent)
{
    sock = nullptr;
    m_offlineMode = true;
}

SmartHomeClient::~SmartHomeClient()
{
    if(sock) {
        sock->deleteLater();
    }
    m_instance = nullptr;
}

SmartHomeClient* SmartHomeClient::instance()
{
    if(!m_instance) {
        m_instance = new SmartHomeClient();
        qDebug() << "SmartHomeClient instance created";
    }
    return m_instance;
}

bool SmartHomeClient::auth(const QString &login, const QString &password)
{
    if (m_offlineMode) {
        // Заглушка для работы без сервера
        if (login == "admin" && password == "123456") {
            emit authSuccess();
            return true;
        }
        emit errorOccurred("Неверный логин или пароль");
        return false;
    }

    // Здесь может быть код для реального сервера
    emit errorOccurred("Сервер недоступен");
    return false;
}

QString SmartHomeClient::msg2serv(QString msg)
{
    ///!исправить
    return msg;
}

bool SmartHomeClient::reg(const QString &login, const QString &password,
                          const QString &name, const QString &email)
{
    if (m_offlineMode) {
        // Простая проверка данных
        if (login.isEmpty() || password.isEmpty() || name.isEmpty()) {
            emit errorOccurred("Заполните все обязательные поля");
            return false;
        }

        if (password.length() < 6) {
            emit errorOccurred("Пароль должен быть не менее 6 символов");
            return false;
        }

        // В режиме заглушки всегда возвращаем успех
        emit regSuccess();
        return true;
    }

    emit errorOccurred("Сервер недоступен");
    return false;
}
