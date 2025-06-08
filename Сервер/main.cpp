#include <QCoreApplication>
#include <QDebug>
#include "tcp_server.h"
#include "DatabaseSingleton.h"

#ifdef BUILD_TESTS
#include <QtTest>
#include "test_database_singleton.h"
#endif

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

#ifdef BUILD_TESTS
    QStringList args = a.arguments();
    if (args.contains("--selftest")) {
        // Удаляем --selftest из аргументов до передачи в QTest
        QStringList testArgs = args;
        testArgs.removeAll("--selftest");

        // Преобразуем QStringList обратно в argc/argv
        QVector<QByteArray> argStorage;
        QVector<char*> testArgv;
        for (const QString &arg : testArgs) {
            argStorage.append(arg.toLocal8Bit());
            testArgv.append(argStorage.last().data());
        }
        int testArgc = testArgv.size();

        TestDatabaseSingleton test;
        return QTest::qExec(&test, testArgc, testArgv.data());
    }
#endif

    qDebug() << "MAIN: 1";
    DatabaseSingleton::instance().initializeDatabase();
    qDebug() << "MAIN: 2";

    TcpServer server;
    qDebug() << "MAIN: 3";

    if (!server.startServer()) {
        qCritical() << "Server could not start!";
        return 1;
    }
    qDebug() << "MAIN: 4";

    return a.exec();
}
// ./SmartHomeServer --selftest запуск тестов
