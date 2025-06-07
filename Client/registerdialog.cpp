#include "registerdialog.h"
#include "ui_registerdialog.h"
#include <QMessageBox>

RegisterDialog::RegisterDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::RegisterDialog),
    client(new SmartHomeClient(this))
{
    ui->setupUi(this);
    client->connectToServer("127.0.0.1", 12345); // адрес сервера

    connect(client, &SmartHomeClient::serverMessage, this, &RegisterDialog::onRegisterResult);
    connect(ui->registerButton, &QPushButton::clicked, this, &RegisterDialog::on_registerButton_clicked);
    connect(ui->cancelButton, &QPushButton::clicked, this, &RegisterDialog::on_cancelButton_clicked);
    connect(ui->showPasswordCheckBox, &QCheckBox::stateChanged, this, &RegisterDialog::on_showPasswordCheckBox_stateChanged);
}

RegisterDialog::~RegisterDialog()
{
    delete ui;
}

void RegisterDialog::on_registerButton_clicked()
{
    QString login = ui->loginLineEdit->text();
    QString email = ui->emailLineEdit->text();
    QString password = ui->passwordLineEdit->text();
    QString confirm = ui->confirmPasswordLineEdit->text();

    if (login.isEmpty() || email.isEmpty() || password.isEmpty() || confirm.isEmpty()) {
        QMessageBox::warning(this, "Ошибка", "Пожалуйста, заполните все поля.");
        return;
    }
    if (password != confirm) {
        QMessageBox::warning(this, "Ошибка", "Пароли не совпадают");
        return;
    }
    // Можно добавить базовую валидацию email
    if (!email.contains("@") || !email.contains(".")) {
        QMessageBox::warning(this, "Ошибка", "Некорректный email");
        return;
    }
    // Вызов регистрации с email (доработай registerUser если нужно)
    client->registerUser(login, password, email); // предполагается, что метод принимает email
}

void RegisterDialog::on_cancelButton_clicked()
{
    reject();
}

void RegisterDialog::on_showPasswordCheckBox_stateChanged(int state)
{
    auto mode = (state == Qt::Checked) ? QLineEdit::Normal : QLineEdit::Password;
    ui->passwordLineEdit->setEchoMode(mode);
    ui->confirmPasswordLineEdit->setEchoMode(mode);
}

void RegisterDialog::onRegisterResult(const QJsonObject &msg)
{
    QString type = msg.value("type").toString();
    if (type == "registration_result") {
        bool ok = msg.value("ok").toBool();
        if (ok) {
            QMessageBox::information(this, "Готово", "Вы успешно зарегистрированы!");
            accept();
        } else {
            QMessageBox::warning(this, "Ошибка регистрации", msg.value("reason").toString());
        }
    }
}
