#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QJsonObject>
#include <QSettings>
#include <QRandomGenerator>
#include <QDebug>
#include <QTimer>
#include <QMessageBox>

QVector<QTime> MainWindow::randomTimes;
int MainWindow::currentTimeIndex = 0;

MainWindow::MainWindow(SmartHomeClient *client, QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow), client(client)
{
    ui->setupUi(this);

    // Используем timeACStart (QTimeEdit для времени включения кондиционера)
    m_acScheduledTime = ui->timeACStart->time();

    // Таймер для проверки времени
    m_notifyTimer = new QTimer(this);
    connect(m_notifyTimer, &QTimer::timeout, this, &MainWindow::checkScheduledNotifications);
    m_notifyTimer->start(30 * 1000); // Проверять каждые 30 секунд
    qDebug() << "MainWindow создан";

    connect(ui->timeACStart, &QTimeEdit::timeChanged, this, &MainWindow::on_timeACStart_timeChanged);

    connect(ui->btnKettleOn,     &QPushButton::clicked, this, &MainWindow::on_btnKettleOn_clicked);
    connect(ui->sliderKettleTemp,&QSlider::valueChanged, this, &MainWindow::on_sliderKettleTemp_valueChanged);
    connect(ui->btnLightToggle,  &QPushButton::clicked, this, &MainWindow::on_btnLightToggle_clicked);
    connect(ui->btnACOn,         &QPushButton::clicked, this, &MainWindow::on_btnACOn_clicked);
    connect(ui->sliderACTemp,    &QSlider::valueChanged, this, &MainWindow::on_sliderACTemp_valueChanged);
    connect(ui->btnSocketToggle, &QPushButton::clicked, this, &MainWindow::on_btnSocketToggle_clicked);
    connect(ui->btnCurtainOpen,  &QPushButton::clicked, this, &MainWindow::on_btnCurtainOpen_clicked);
    connect(ui->btnCurtainClose, &QPushButton::clicked, this, &MainWindow::on_btnCurtainClose_clicked);
    connect(ui->btnMusicToggle, &QPushButton::clicked, this, &MainWindow::on_btnMusicToggle_clicked);
    connect(ui->sliderMusicVolume, &QSlider::valueChanged, this, &MainWindow::on_sliderMusicVolume_valueChanged);
    // Устанавливаем время из randomTimes
    if (!randomTimes.isEmpty())
        ui->timeACStart->setTime(randomTimes[currentTimeIndex]);

    connect(client, &SmartHomeClient::serverMessage, this, &MainWindow::updateDeviceStatus);

    if (randomTimes.isEmpty()) {
        for (int i = 0; i < 10; ++i) {
            int hour = QRandomGenerator::global()->bounded(0, 24);
            int minute = QRandomGenerator::global()->bounded(0, 60);
            randomTimes.append(QTime(hour, minute));
        }
    }
    QSettings settings("YourCompany", "YourAppName");
    currentTimeIndex = settings.value("RandomTimeIndex", 0).toInt();
    setNextRandomTime();
    settings.setValue("RandomTimeIndex", currentTimeIndex);

    // --- Получить состояние устройств ---
    requestStatus();
}

void MainWindow::setNextRandomTime()
{
    if (!randomTimes.isEmpty()) {
        ui->timeACStart->setTime(randomTimes[currentTimeIndex]);
        currentTimeIndex = (currentTimeIndex + 1) % randomTimes.size();
    }
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::requestStatus()
{
    QJsonObject cmd;
    cmd["type"] = "get_status";
    client->sendCommand(cmd);
}

void MainWindow::on_btnKettleOn_clicked()
{
    QJsonObject cmd;
    cmd["type"] = "control_kettle";
    QJsonObject set;
    set["on"] = !m_kettleOn;
    set["temp"] = ui->sliderKettleTemp->value();
    cmd["settings"] = set;
    client->sendCommand(cmd);

    requestStatus();
}

void MainWindow::on_sliderKettleTemp_valueChanged(int value)
{
    ui->labelKettleTemp->setText(QString("%1°C").arg(value));
    if (m_kettleOn) {
        QJsonObject cmd;
        cmd["type"] = "control_kettle";
        QJsonObject set;
        set["on"] = true;
        set["temp"] = value;
        cmd["settings"] = set;
        client->sendCommand(cmd);

        requestStatus();
    }
}

void MainWindow::on_btnLightToggle_clicked()
{
    QJsonObject cmd;
    cmd["type"] = "control_light";
    QJsonObject set;
    set["on"] = !m_lightOn;
    cmd["settings"] = set;
    client->sendCommand(cmd);

    requestStatus();
}

void MainWindow::on_btnACOn_clicked()
{
    QJsonObject cmd;
    cmd["type"] = "control_ac";
    QJsonObject set;
    set["on"] = !m_acOn;
    set["temp"] = ui->sliderACTemp->value();
    set["mode"] = m_acMode;
    cmd["settings"] = set;
    client->sendCommand(cmd);

    requestStatus();
}

void MainWindow::on_sliderACTemp_valueChanged(int value)
{
    ui->labelACTemp->setText(QString("%1°C").arg(value));
    if (m_acOn) {
        QJsonObject cmd;
        cmd["type"] = "control_ac";
        QJsonObject set;
        set["on"] = true;
        set["temp"] = value;
        set["mode"] = m_acMode;
        cmd["settings"] = set;
        client->sendCommand(cmd);

        requestStatus();
    }
}

void MainWindow::on_btnSocketToggle_clicked()
{
    QJsonObject cmd;
    cmd["type"] = "control_socket";
    QJsonObject set;
    set["on"] = !m_socketOn;
    cmd["settings"] = set;
    client->sendCommand(cmd);

    requestStatus();
}

void MainWindow::on_btnCurtainOpen_clicked()
{
    QJsonObject cmd;
    cmd["type"] = "control_curtain";
    QJsonObject set;
    set["action"] = "open";
    cmd["settings"] = set;
    client->sendCommand(cmd);

    requestStatus();
}

void MainWindow::on_btnCurtainClose_clicked()
{
    QJsonObject cmd;
    cmd["type"] = "control_curtain";
    QJsonObject set;
    set["action"] = "close";
    cmd["settings"] = set;
    client->sendCommand(cmd);

    requestStatus();
}
void MainWindow::on_btnMusicToggle_clicked()
{
    QJsonObject cmd;
    cmd["type"] = "control_music";
    QJsonObject set;
    set["on"] = !m_musicOn;
    set["volume"] = ui->sliderMusicVolume->value();
    cmd["settings"] = set;
    client->sendCommand(cmd);

    requestStatus();
}

void MainWindow::on_sliderMusicVolume_valueChanged(int value)
{
    ui->labelMusicVolume->setText(QString("Громкость: %1%").arg(value));
    // Можно отправлять команду только если музыка включена
    if (m_musicOn) {
        QJsonObject cmd;
        cmd["type"] = "control_music";
        QJsonObject set;
        set["on"] = true;
        set["volume"] = value;
        cmd["settings"] = set;
        client->sendCommand(cmd);

        requestStatus();
    }
}
void MainWindow::updateDeviceStatus(const QJsonObject &msg)
{
    if (msg.value("type").toString() != "status")
        return;
    QJsonObject devices = msg.contains("devices") ? msg["devices"].toObject() : msg;

    // Чайник
    bool kettleOn = devices.value("kettle").toBool();
    if (kettleOn != m_prevKettleOn)
        QMessageBox::information(this, "Уведомление", kettleOn ? "Чайник включён!" : "Чайник выключен!");
    m_prevKettleOn = kettleOn;
    m_kettleOn = kettleOn;
    m_kettleTemp = devices.value("kettle_temp").toInt(ui->sliderKettleTemp->value());
    ui->labelKettleStatus2->setText(m_kettleOn ? "Включён" : "Выключен");
    ui->btnKettleOn->setText(m_kettleOn ? "Выключить" : "Включить");
    ui->sliderKettleTemp->setValue(m_kettleTemp);
    ui->labelKettleTemp->setText(QString("%1°C").arg(m_kettleTemp));

    // Свет
    bool lightOn = devices.value("light").toBool();
    if (lightOn != m_prevLightOn)
        QMessageBox::information(this, "Уведомление", lightOn ? "Свет включён!" : "Свет выключен!");
    m_prevLightOn = lightOn;
    m_lightOn = lightOn;
    ui->labelLightStatus2->setText(m_lightOn ? "Вкл" : "Выкл");
    ui->btnLightToggle->setText(m_lightOn ? "Выключить" : "Включить");

    // Кондиционер
    bool acOn = devices.value("ac_on").toBool();
    if (acOn != m_prevACOn)
        QMessageBox::information(this, "Уведомление", acOn ? "Кондиционер включён!" : "Кондиционер выключен!");
    m_prevACOn = acOn;
    m_acOn = acOn;
    m_acTemp = devices.value("ac_temperature").toInt(ui->sliderACTemp->value());
    m_acMode = devices.value("ac_mode").toString();
    ui->labelACStatus2->setText(m_acOn ? "Вкл" : "Выкл");
    ui->btnACOn->setText(m_acOn ? "Выключить" : "Включить");
    ui->sliderACTemp->setValue(m_acTemp);
    ui->labelACTemp->setText(QString("%1°C").arg(m_acTemp));

    // Розетка
    bool socketOn = devices.value("socket").toBool();
    if (socketOn != m_prevSocketOn)
        QMessageBox::information(this, "Уведомление", socketOn ? "Розетка включена!" : "Розетка выключена!");
    m_prevSocketOn = socketOn;
    m_socketOn = socketOn;
    ui->labelSocketStatus2->setText(m_socketOn ? "Вкл" : "Выкл");
    ui->btnSocketToggle->setText(m_socketOn ? "Выключить" : "Включить");

    // Шторы
    bool curtainOpen = devices.value("curtain").toBool();
    if (curtainOpen != m_prevCurtainOpen)
        QMessageBox::information(this, "Уведомление", curtainOpen ? "Шторы открыты!" : "Шторы закрыты!");
    m_prevCurtainOpen = curtainOpen;
    m_curtainOpen = curtainOpen;
    ui->labelCurtainStatus2->setText(m_curtainOpen ? "Открыты" : "Закрыты");

    // Музыка
    // Музыка
    bool musicOn = devices.value("music_on").toBool();
    if (musicOn != m_prevMusicOn)
        QMessageBox::information(this, "Уведомление", musicOn ? "Музыка включена!" : "Музыка выключена!");
    m_prevMusicOn = musicOn;
    m_musicOn = musicOn;
    m_musicVolume = devices.value("music_volume").toInt(ui->sliderMusicVolume->value());
    ui->labelMusicStatus2->setText(m_musicOn ? "Включена" : "Выключена");
    ui->btnMusicToggle->setText(m_musicOn ? "Выключить" : "Включить");


}
void MainWindow::checkScheduledNotifications()
{
    QTime now = QTime::currentTime();
    if (now.hour() == m_acScheduledTime.hour() && now.minute() == m_acScheduledTime.minute()) {
        QMessageBox::information(this, "Уведомление", "Кондиционер включится в " + m_acScheduledTime.toString("HH:mm"));
        // Можно добавить флаг, чтобы не спамить каждую минуту
    }
}

void MainWindow::on_timeACStart_timeChanged(const QTime &time)
{
    m_acScheduledTime = time;
}
