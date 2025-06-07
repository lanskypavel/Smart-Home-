#pragma once

#include <QDialog>
#include "smart_home_client.h"

namespace Ui { class RegisterDialog; }

class RegisterDialog : public QDialog
{
    Q_OBJECT
public:
    explicit RegisterDialog(QWidget *parent = nullptr);
    ~RegisterDialog();

private slots:
    void on_registerButton_clicked();
    void on_cancelButton_clicked();
    void on_showPasswordCheckBox_stateChanged(int);

    void onRegisterResult(const QJsonObject &msg);

private:
    Ui::RegisterDialog *ui;
    SmartHomeClient *client;
};
