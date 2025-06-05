#ifndef DATABASESINGLETON_H
#define DATABASESINGLETON_H

#include <QObject>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QMutex>
#include <QDebug>
#include <QString>
#include <QVariant>

class DatabaseSingleton
{
public:
    static DatabaseSingleton& instance()
    {
        static DatabaseSingleton instance;
        return instance;
    }

    bool initializeDatabase()
    {
        QString connectionName = "main_connection";
        QSqlDatabase db;
        if (QSqlDatabase::contains(connectionName)) {
            db = QSqlDatabase::database(connectionName);
        } else {
            db = QSqlDatabase::addDatabase("QSQLITE", connectionName);
            db.setDatabaseName("smart_home.db");
        }

        if(!db.open()) {
            qCritical() << "Cannot open database:" << db.lastError();
            return false;
        }

        QSqlQuery query(db);
        QString createUsersTable =
            "CREATE TABLE IF NOT EXISTS users ("
            "id INTEGER PRIMARY KEY AUTOINCREMENT, "
            "username TEXT UNIQUE NOT NULL, "
            "password TEXT NOT NULL);";

        if(!query.exec(createUsersTable)) {
            qCritical() << "DB error:" << query.lastError();
            return false;
        }
        return true;
    }

    bool registerUser(const QString& username, const QString& password) {
        qDebug() << "=== MY DEBUG: registerUser called ===";
        QSqlDatabase db = QSqlDatabase::database("main_connection");
        QSqlQuery query(db);
        query.prepare("INSERT INTO users (username, password) VALUES (?, ?)");
        query.addBindValue(username);
        query.addBindValue(password);
        if (!query.exec()) {
            qDebug() << "registerUser error:" << query.lastError().text();
            qDebug() << "registerUser driver error:" << query.lastError().driverText();
            qDebug() << "registerUser db error:" << query.lastError().databaseText();
            return false;
        }
        qDebug() << "=== MY DEBUG: registerUser success ===";
        return true;
    }

    bool authenticateUser(const QString &username, const QString &password)
    {
        QMutexLocker locker(&m_mutex);
        QSqlDatabase db = QSqlDatabase::database("main_connection");
        QSqlQuery query(db);
        query.prepare("SELECT * FROM users WHERE username = :user AND password = :pass");
        query.bindValue(":user", QVariant(username));
        query.bindValue(":pass", QVariant(password));
        return query.exec() && query.next();
    }

private:
    DatabaseSingleton() {}
    ~DatabaseSingleton() {}
    DatabaseSingleton(const DatabaseSingleton&) = delete;
    DatabaseSingleton& operator=(const DatabaseSingleton&) = delete;
    QMutex m_mutex;
    void getStatus();
};

#endif // DATABASESINGLETON_H
