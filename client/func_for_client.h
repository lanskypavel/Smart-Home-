#ifndef FUNC_FOR_CLIENT_H
#define FUNC_FOR_CLIENT_H

#include <QString>
#include <QMessageBox>

// Проверка авторизации
bool auth(const QString &login, const QString &password);

// Регистрация нового пользователя
bool reg(const QString &login, const QString &password,
         const QString &name, const QString &email);

#endif // FUNC_FOR_CLIENT_H
