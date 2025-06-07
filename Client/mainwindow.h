#pragma once

#include <QMainWindow>
#include "smart_home_client.h"
#include <QTimer>
#include <QTime>

namespace Ui { class MainWindow; }

class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    explicit MainWindow(SmartHomeClient *client, QWidget *parent = nullptr);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    SmartHomeClient *client;

    // состояния устройств
    bool m_kettleOn = false;
    int m_kettleTemp = 80;
    bool m_lightOn = false;
    bool m_acOn = false;
    int m_acTemp = 22;
    QString m_acMode;
    bool m_socketOn = false;
    bool m_curtainOpen = false;

    // Для рандомного времени
    static QVector<QTime> randomTimes;
    static int currentTimeIndex;
    void setNextRandomTime();

    // Для музыки
    bool m_musicOn = false;
    int m_musicVolume = 50;

    // Для уведомлений о состоянии устройств
    bool m_prevKettleOn = false;
    bool m_prevACOn = false;

    bool m_prevLightOn = false;
    bool m_prevSocketOn = false;
    bool m_prevCurtainOpen = false;
    bool m_prevMusicOn = false;

    // Для уведомления о времени
    QTime m_acScheduledTime;
    QTimer *m_notifyTimer = nullptr;

private slots:
    void on_btnKettleOn_clicked();
    void on_sliderKettleTemp_valueChanged(int value);
    void on_btnLightToggle_clicked();
    void on_btnACOn_clicked();
    void on_sliderACTemp_valueChanged(int value);
    void on_btnSocketToggle_clicked();
    void on_btnCurtainOpen_clicked();
    void on_btnCurtainClose_clicked();
    void updateDeviceStatus(const QJsonObject &msg);
    void on_timeACStart_timeChanged(const QTime &time);
    void checkScheduledNotifications();
    void on_btnMusicToggle_clicked();
    void on_sliderMusicVolume_valueChanged(int value);
    void requestStatus();
};
