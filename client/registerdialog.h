#ifndef REGISTERDIALOG_H
#define REGISTERDIALOG_H

#include <QDialog>

namespace Ui {
class RegisterDialog;
}

class RegisterDialog : public QDialog
{
    Q_OBJECT

public:
    explicit RegisterDialog(QWidget *parent = nullptr);
    ~RegisterDialog();

signals:
    void reg_ok();

private slots:
    void on_registerButton_clicked();
    void on_showPasswordCheckBox_stateChanged(int state);
    void handleRegSuccess();
    void handleRegError(const QString &message);

private:
    Ui::RegisterDialog *ui;
    bool isProcessing = false;
    void showResult(bool success, const QString &message);
};

#endif // REGISTERDIALOG_H
