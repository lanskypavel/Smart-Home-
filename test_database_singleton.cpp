#include "test_database_singleton.h"
#include <QtTest>
#include "DatabaseSingleton.h"
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>

void TestDatabaseSingleton::initTestCase() {
    QVERIFY(DatabaseSingleton::instance().initializeDatabase());
}

void TestDatabaseSingleton::cleanup() {
    qDebug() << "cleanup called";
    QSqlDatabase db = QSqlDatabase::database("main_connection");
    QSqlQuery query(db);
    if (!query.exec("DELETE FROM users")) {
        qWarning() << "Failed to clear users table:" << query.lastError();
    } else {
        qDebug() << "Users table cleared successfully.";
    }
    // Проверяем содержимое таблицы после очистки
    QSqlQuery check("SELECT username FROM users", db);
    bool empty = true;
    while (check.next()) {
        qDebug() << "User in DB after cleanup:" << check.value(0).toString();
        empty = false;
    }
    if (empty) {
        qDebug() << "Users table is empty after cleanup.";
    }
}

void TestDatabaseSingleton::testRegisterUser() {
    QSqlDatabase db = QSqlDatabase::database("main_connection");
    QSqlQuery clear(db);
    if (!clear.exec("DELETE FROM users")) {
        qWarning() << "Failed to clear users table at test start:" << clear.lastError();
    }

    qDebug() << "testRegisterUser started";
    qDebug() << "=== MY DEBUG: testRegisterUser started ===";
    bool ok = DatabaseSingleton::instance().registerUser("user1", "pass1");
    if (!ok) {
        qDebug() << "=== MY DEBUG: registerUser returned false ===";
    } else {
        qDebug() << "=== MY DEBUG: registerUser returned true ===";
    }
    QVERIFY(ok);
    QVERIFY(!DatabaseSingleton::instance().registerUser("user1", "pass1"));
}

bool ok = DatabaseSingleton::instance().registerUser("user1", "pass1");
    if (!ok) {
        qWarning() << "REGISTER ERROR: Не удалось зарегистрировать user1/pass1!";
        // Попробуем вывести ошибку SQL напрямую
        QSqlQuery debugQuery("SELECT * FROM users", db);
        while (debugQuery.next()) {
            qDebug() << "User in DB after failed registration:" << debugQuery.value(0).toString();
        }
    }
    QVERIFY(ok);
    QVERIFY(!DatabaseSingleton::instance().registerUser("user1", "pass1"));
}

void TestDatabaseSingleton::testAuthenticateUser() {
    QVERIFY(DatabaseSingleton::instance().registerUser("user1", "pass1"));
    QVERIFY(DatabaseSingleton::instance().authenticateUser("user1", "pass1"));
    QVERIFY(!DatabaseSingleton::instance().authenticateUser("user1", "badpass"));
    QVERIFY(!DatabaseSingleton::instance().authenticateUser("nouser", "pass"));
}

void TestDatabaseSingleton::cleanupTestCase() {
    // Можно добавить финальную очистку, если нужно
}
