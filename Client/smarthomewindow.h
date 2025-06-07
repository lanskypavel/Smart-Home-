#pragma once

#include <QMainWindow>
#include <QJsonObject>
#include "smart_home_client.h"

namespace Ui { class SmarthomeWindow; }

class SmarthomeWindow : public QMainWindow
{
    Q_OBJECT
public:
    explicit SmarthomeWindow(QWidget *parent = nullptr);
    ~SmarthomeWindow();

private slots:
    void on_loginButton_clicked();
    void on_registerButton_clicked();
    void on_showPasswordCheckBox_stateChanged(int state);
    void onServerMessage(const QJsonObject &msg);

private:
    Ui::SmarthomeWindow *ui;
    SmartHomeClient *client;
};
