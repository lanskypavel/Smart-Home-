#ifndef SMART_HOME_CLIENT_H
#define SMART_HOME_CLIENT_H

#include <QObject>
#include <QTcpSocket>
#include <QJsonObject>
#include <QJsonDocument>

class SmartHomeClient : public QObject
{
    Q_OBJECT
private:
    explicit SmartHomeClient(QObject *parent = nullptr); // Приватный конструктор
    static SmartHomeClient* m_instance; // Статический экземпляр

public:
    static SmartHomeClient* instance(); // Статический метод для получения экземпляра
    ~SmartHomeClient();

    bool auth(const QString &login, const QString &password);
    bool reg(const QString &login, const QString &password,
             const QString &name, const QString &email);

signals:
    void authSuccess();
    void regSuccess();
    void errorOccurred(const QString& message);

private:
    QTcpSocket *sock;
    QString msg2serv(QString msg);
    bool m_offlineMode = true;
};

#endif // SMART_HOME_CLIENT_H
