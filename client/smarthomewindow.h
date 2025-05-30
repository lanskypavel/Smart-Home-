#ifndef SMARTHOMEWINDOW_H
#define SMARTHOMEWINDOW_H

#include <QMainWindow>

// Предварительное объявление UI-класса
namespace Ui {
class SmarthomeWindow;
}

class SmarthomeWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit SmarthomeWindow(QWidget *parent = nullptr);
    ~SmarthomeWindow();

    // Объявляем сигналы
signals:
    void show_reg();
    void login_success();

private slots:
    void on_loginButton_clicked();
    void on_registerButton_clicked();

private:
    Ui::SmarthomeWindow *ui;
};

#endif // SMARTHOMEWINDOW_H
