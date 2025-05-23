#include "databasesingleton.h"

// Реализация методов класса DatabaseSingleton
DatabaseSingleton& DatabaseSingleton::instance()
{
    static DatabaseSingleton instance;
    return instance;
}

bool DatabaseSingleton::initializeDatabase()
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

// Остальные методы уже реализованы в заголовочном файле (inline)
