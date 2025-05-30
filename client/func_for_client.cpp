#include "func_for_client.h"
#include "smart_home_client.h"

bool auth(const QString &login, const QString &password) {
    return SmartHomeClient::instance()->auth(login, password);
}

bool reg(const QString &login, const QString &password,
         const QString &name, const QString &email) {
    return SmartHomeClient::instance()->reg(login, password, name, email);
}
