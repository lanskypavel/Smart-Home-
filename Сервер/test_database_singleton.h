#pragma once
#include <QObject>

class TestDatabaseSingleton : public QObject
{
    Q_OBJECT
private slots:
    void initTestCase();
    void cleanup();
    void testRegisterUser();
    void testAuthenticateUser();
    void cleanupTestCase();
};
