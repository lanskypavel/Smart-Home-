#include "smarthomewindow.h"
#include "ui_smarthomewindow.h"
#include <QMessageBox>
#include "smart_home_client.h"
#include "mainwindow.h"
#include "registerdialog.h"

SmarthomeWindow::SmarthomeWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::SmarthomeWindow)
{
    ui->setupUi(this);

    client = new SmartHomeClient(this);
    client->connectToServer("127.0.0.1", 12345);

    connect(ui->loginButton, &QPushButton::clicked, this, &SmarthomeWindow::on_loginButton_clicked);
    connect(ui->registerButton, &QPushButton::clicked, this, &SmarthomeWindow::on_registerButton_clicked);
    connect(ui->showPasswordCheckBox, &QCheckBox::stateChanged, this, &SmarthomeWindow::on_showPasswordCheckBox_stateChanged);
    qDebug() << "client pointer:" << client;
    connect(client, &SmartHomeClient::serverMessage, this, &SmarthomeWindow::onServerMessage);
    qDebug() << "SmarthomeWindow создан, сигнал serverMessage подключён";
}

SmarthomeWindow::~SmarthomeWindow()
{
    delete ui;
}

void SmarthomeWindow::on_loginButton_clicked()
{
    qDebug() << "on_loginButton_clicked вызван!";
    QString login = ui->loginLineEdit->text();
    QString password = ui->passwordLineEdit->text();
    if (login.isEmpty() || password.isEmpty()) {
        QMessageBox::warning(this, "Ошибка", "Введите логин и пароль!");
        return;
    }
    ui->loginButton->setEnabled(false); // <--- Отключаем!
    client->login(login, password);
}

void SmarthomeWindow::onServerMessage(const QJsonObject &msg)
{
    qDebug() << "Получено сообщение от сервера:" << msg;
    if (msg.value("type").toString() == "auth_result") {
        ui->loginButton->setEnabled(true); // <- Включаем обратно
        if (msg.value("ok").toBool()) {
            qDebug() << "Login OK, opening menu";
            MainWindow *mw = new MainWindow(client);
            mw->show();
            this->close();
        } else {
            QMessageBox::warning(this, "Ошибка входа", msg.value("reason").toString());
        }
    }


}

void SmarthomeWindow::on_showPasswordCheckBox_stateChanged(int state)
{
    ui->passwordLineEdit->setEchoMode(state == Qt::Checked ? QLineEdit::Normal : QLineEdit::Password);
}

void SmarthomeWindow::on_registerButton_clicked()
{


    RegisterDialog reg(this);
    reg.exec();
}
