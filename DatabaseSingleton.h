#ifndef DATABASESINGLETON_H
#define DATABASESINGLETON_H

#include <QObject>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QMutex>
#include <QDebug>

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
        QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
        db.setDatabaseName("smart_home.db");

        if(!db.open()) {
            qCritical() << "Cannot open database:" << db.lastError();
            return false;
        }

        QSqlQuery query;
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

    bool registerUser(const QString &username, const QString &password)
    {
        QMutexLocker locker(&m_mutex);
        QSqlQuery query;
        query.prepare("INSERT INTO users (username, password) VALUES (:user, :pass)");
        query.bindValue(":user", username);
        query.bindValue(":pass", password);
        return query.exec();
    }

    bool authenticateUser(const QString &username, const QString &password)
    {
        QMutexLocker locker(&m_mutex);
        QSqlQuery query;
        query.prepare("SELECT * FROM users WHERE username = :user AND password = :pass");
        query.bindValue(":user", username);
        query.bindValue(":pass", password);
        return query.exec() && query.next();
    }

private:
    DatabaseSingleton() {}
    ~DatabaseSingleton() {}
    DatabaseSingleton(const DatabaseSingleton&) = delete;
    DatabaseSingleton& operator=(const DatabaseSingleton&) = delete;
    QMutex m_mutex;
};

#endif // DATABASESINGLETON_H
