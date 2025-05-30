#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMessageBox>
#include <QMediaPlayer>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_logoutButton_clicked();

    // Управление кондиционером
    void on_btnACSwitch_clicked();
    void on_btnClimateControl_clicked();
    void on_sliderACSetTemp_valueChanged(int value);
    void on_comboACMode_currentIndexChanged(int index);

    // Управление шторами
    void on_btnCurtainOpen_clicked();
    void on_btnCurtainClose_clicked();
    void on_btnCurtainSchedule_clicked();

    // Управление чайником
    void on_btnKettleOn_clicked();
    void on_btnKettleSchedule_clicked();
    void on_sliderKettleTemp_valueChanged(int value);

    // Управление светом
    void on_btnLightSwitch_clicked();
    void on_btnLightSchedule_clicked();

    // Управление розеткой
    void on_btnSocketSwitch_clicked();
    void on_btnSocketSchedule_clicked();

    // Управление музыкой
    void on_playButton_clicked();
    void on_nextButton_clicked();
    void on_prevButton_clicked();
    void on_volumeSlider_valueChanged(int value);
    void updatePlayerPosition(qint64 position);
    void updatePlayerDuration(qint64 duration);
    void on_playlistComboBox_currentIndexChanged(int index);

    // Новый слот для случайного времени
    void on_btnRandomTime_clicked();

private:
    Ui::MainWindow *ui;
    QMediaPlayer *player;

    // Состояния устройств
    bool acState = false;
    bool curtainState = false;
    bool kettleState = false;
    bool lightState = false;
    bool socketState = false;

    void updateDeviceStates();
    void setupMusicPlayer();
    QTime generateRandomTime(); // Новый метод для генерации случайного времени

signals:
    void show_auth();
};

#endif // MAINWINDOW_H
