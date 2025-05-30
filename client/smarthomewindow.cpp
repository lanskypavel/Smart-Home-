#include "smarthomewindow.h"
#include "ui_smarthomewindow.h"
#include "smart_home_client.h"
#include "registerdialog.h"
#include <QMessageBox>
#include <QDebug>

SmarthomeWindow::SmarthomeWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::SmarthomeWindow)
{
    ui->setupUi(this);
    ui->passwordLineEdit->setEchoMode(QLineEdit::Password);

    // Подключение сигналов
    connect(SmartHomeClient::instance(), &SmartHomeClient::authSuccess,
            this, &SmarthomeWindow::login_success);
}

SmarthomeWindow::~SmarthomeWindow()
{
    delete ui;
}

void SmarthomeWindow::on_loginButton_clicked()
{
    QString login = ui->loginLineEdit->text();
    QString password = ui->passwordLineEdit->text();

    if (SmartHomeClient::instance()->auth(login, password)) {
        qDebug() << "Login successful";
    } else {
        QMessageBox::warning(this, "Error", "Invalid login or password");
    }
}

void SmarthomeWindow::on_registerButton_clicked()
{
    RegisterDialog regDialog(this);
    connect(&regDialog, &RegisterDialog::reg_ok, this, [this](){
        qDebug() << "Registration completed successfully";
    });
    regDialog.exec();
}
