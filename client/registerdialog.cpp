#include "registerdialog.h"
#include "ui_registerdialog.h"
#include "smart_home_client.h"
#include <QMessageBox>
#include <QString>
#include <QDebug>

RegisterDialog::RegisterDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::RegisterDialog),
    isProcessing(false)
{
    ui->setupUi(this);
    ui->passwordLineEdit->setEchoMode(QLineEdit::Password);
    ui->confirmPasswordLineEdit->setEchoMode(QLineEdit::Password);

    connect(SmartHomeClient::instance(), &SmartHomeClient::regSuccess,
            this, &RegisterDialog::handleRegSuccess);
    connect(SmartHomeClient::instance(), &SmartHomeClient::errorOccurred,
            this, &RegisterDialog::handleRegError);
}

RegisterDialog::~RegisterDialog()
{
    delete ui;
}

void RegisterDialog::on_registerButton_clicked()
{
    if(isProcessing) return;
    isProcessing = true;
    ui->registerButton->setEnabled(false);

    QString login = ui->loginLineEdit->text().trimmed();
    QString password = ui->passwordLineEdit->text();
    QString confirm = ui->confirmPasswordLineEdit->text();
    QString name = ui->nameLineEdit->text().trimmed();
    QString email = ui->emailLineEdit->text().trimmed();

    // Валидация данных
    if(login.isEmpty() || password.isEmpty() || name.isEmpty()) {
        showResult(false, "Заполните все обязательные поля!");
        return;
    }

    if(password.length() < 6) {
        showResult(false, "Пароль должен быть не менее 6 символов");
        return;
    }

    if(password != confirm) {
        showResult(false, "Пароли не совпадают!");
        return;
    }

    qDebug() << "Попытка регистрации:" << login;
    SmartHomeClient::instance()->reg(login, password, name, email);
}

void RegisterDialog::handleRegSuccess()
{
    showResult(true, "Регистрация завершена успешно!");
    emit reg_ok();
    close();
}

void RegisterDialog::handleRegError(const QString &message)
{
    showResult(false, message.isEmpty() ? "Ошибка регистрации" : message);
}

void RegisterDialog::showResult(bool success, const QString &message)
{
    isProcessing = false;
    ui->registerButton->setEnabled(true);

    if(success) {
        QMessageBox::information(this, "Успех", message);
    } else {
        QMessageBox::warning(this, "Ошибка", message);
    }
}

void RegisterDialog::on_showPasswordCheckBox_stateChanged(int state)
{
    QLineEdit::EchoMode mode = state ? QLineEdit::Normal : QLineEdit::Password;
    ui->passwordLineEdit->setEchoMode(mode);
    ui->confirmPasswordLineEdit->setEchoMode(mode);
}
