#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QTime>
#include <QMessageBox>
#include <QFileInfo>
#include <QDir>
#include <QRandomGenerator>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    player(new QMediaPlayer(this))
{
    ui->setupUi(this);

    // Установка тёмно-серого фона и белого текста
    this->setStyleSheet(
        "background-color: #333333;"
        "color: white;"
        "QPushButton { background-color: #555555; color: white; }"
        "QLabel { color: white; }"
        "QComboBox { background-color: #555555; color: white; }"
        "QSlider { background-color: #555555; }"
        "QTimeEdit { background-color: #555555; color: white; }"
    );

    this->setWindowTitle("Умный дом - Главное меню");

    // Установка формата времени для всех QTimeEdit
    ui->timeCurtain->setDisplayFormat("HH:mm");
    ui->timeKettle->setDisplayFormat("HH:mm");
    ui->timeLight->setDisplayFormat("HH:mm");
    ui->timeSocket->setDisplayFormat("HH:mm");

    // Инициализация интерфейса устройств
    ui->comboACMode->addItems({"Охлаждение", "Обогрев", "Вентиляция", "Авто"});
    ui->sliderACSetTemp->setRange(16, 30);
    ui->sliderACSetTemp->setValue(22);
    ui->sliderKettleTemp->setRange(30, 100);
    ui->sliderKettleTemp->setValue(90);

    // Установка текущего времени для всех таймеров
    QTime currentTime = QTime::currentTime();
    ui->timeCurtain->setTime(currentTime);
    ui->timeKettle->setTime(currentTime);
    ui->timeLight->setTime(currentTime);
    ui->timeSocket->setTime(currentTime);

    updateDeviceStates();
    setupMusicPlayer();
}

MainWindow::~MainWindow()
{
    delete player;
    delete ui;
}

void MainWindow::setupMusicPlayer()
{
    player->setVolume(50);
    ui->volumeSlider->setValue(50);
    ui->volumeLabel->setText("Громкость: 50%");

    ui->playlistComboBox->addItems({
        "Песня 1",
        "Песня 2",
        "Песня 3"
    });

    connect(player, &QMediaPlayer::positionChanged, this, &MainWindow::updatePlayerPosition);
    connect(player, &QMediaPlayer::durationChanged, this, &MainWindow::updatePlayerDuration);
}

void MainWindow::updateDeviceStates()
{
    ui->btnACSwitch->setText(acState ? "Выключить" : "Включить");
    ui->btnCurtainOpen->setEnabled(!curtainState);
    ui->btnCurtainClose->setEnabled(curtainState);
    ui->btnKettleOn->setText(kettleState ? "Выключить чайник" : "Включить чайник");
    ui->btnLightSwitch->setText(lightState ? "Выключить свет" : "Включить свет");
    ui->btnSocketSwitch->setText(socketState ? "Выключить розетку" : "Включить розетку");

    ui->currentACTemp->setText(acState ? QString::number(ui->sliderACSetTemp->value()) + "°C" : "--");
    ui->currentKettleTemp->setText(kettleState ? QString::number(ui->sliderKettleTemp->value()) + "°C" : "--");
}

// Слоты для управления устройствами (остаются без изменений)
void MainWindow::on_logoutButton_clicked()
{
    this->close();
    emit show_auth();
}

void MainWindow::on_btnACSwitch_clicked()
{
    acState = !acState;
    updateDeviceStates();
    QString status = acState ? "включен" : "выключен";
    QMessageBox::information(this, "Кондиционер", "Кондиционер " + status);
}

void MainWindow::on_btnClimateControl_clicked()
{
    QMessageBox::information(this, "Климат-контроль", "Режим климат-контроля активирован");
}

void MainWindow::on_sliderACSetTemp_valueChanged(int value)
{
    ui->displayACSetTemp->setText(QString::number(value) + "°C";
    if (acState) {
        ui->currentACTemp->setText(QString::number(value) + "°C");
    }
}

void MainWindow::on_comboACMode_currentIndexChanged(int index)
{
    QString mode = ui->comboACMode->itemText(index);
    QMessageBox::information(this, "Режим кондиционера", "Установлен режим: " + mode);
}

void MainWindow::on_btnCurtainOpen_clicked()
{
    curtainState = true;
    updateDeviceStates();
    QMessageBox::information(this, "Шторы", "Шторы открыты");
}

void MainWindow::on_btnCurtainClose_clicked()
{
    curtainState = false;
    updateDeviceStates();
    QMessageBox::information(this, "Шторы", "Шторы закрыты");
}

void MainWindow::on_btnCurtainSchedule_clicked()
{
    QTime time = ui->timeCurtain->time();
    QString action = curtainState ? "закрытия" : "открытия";
    QMessageBox::information(this, "Расписание штор",
                           "Установлено расписание " + action + " штор на " + time.toString("HH:mm"));
}

void MainWindow::on_btnKettleOn_clicked()
{
    kettleState = !kettleState;
    updateDeviceStates();
    QString status = kettleState ? "включен" : "выключен";
    QMessageBox::information(this, "Чайник", "Чайник " + status);
}

void MainWindow::on_btnKettleSchedule_clicked()
{
    QTime time = ui->timeKettle->time();
    QMessageBox::information(this, "Расписание чайника",
                           "Установлено расписание включения чайника на " + time.toString("HH:mm"));
}

void MainWindow::on_sliderKettleTemp_valueChanged(int value)
{
    ui->displayKettleSetTemp->setText(QString::number(value) + "°C");
    if (kettleState) {
        ui->currentKettleTemp->setText(QString::number(value) + "°C");
    }
}

void MainWindow::on_btnLightSwitch_clicked()
{
    lightState = !lightState;
    updateDeviceStates();
    QString status = lightState ? "включен" : "выключен";
    QMessageBox::information(this, "Свет", "Свет " + status);
}

void MainWindow::on_btnLightSchedule_clicked()
{
    QTime time = ui->timeLight->time();
    QString action = lightState ? "выключения" : "включения";
    QMessageBox::information(this, "Расписание света",
                           "Установлено расписание " + action + " света на " + time.toString("HH:mm"));
}

void MainWindow::on_btnSocketSwitch_clicked()
{
    socketState = !socketState;
    updateDeviceStates();
    QString status = socketState ? "включена" : "выключена";
    QMessageBox::information(this, "Розетка", "Умная розетка " + status);
}

void MainWindow::on_btnSocketSchedule_clicked()
{
    QTime time = ui->timeSocket->time();
    QString action = socketState ? "выключения" : "включения";
    QMessageBox::information(this, "Расписание розетки",
                           "Установлено расписание " + action + " розетки на " + time.toString("HH:mm"));
}

void MainWindow::on_playButton_clicked()
{
    if (player->state() == QMediaPlayer::PlayingState) {
        player->pause();
        ui->playButton->setText("▶");
    } else {
        player->play();
        ui->playButton->setText("⏸");
    }
}

void MainWindow::on_nextButton_clicked()
{
    int nextIndex = (ui->playlistComboBox->currentIndex() + 1) % ui->playlistComboBox->count();
    ui->playlistComboBox->setCurrentIndex(nextIndex);
}

void MainWindow::on_prevButton_clicked()
{
    int prevIndex = (ui->playlistComboBox->currentIndex() - 1 + ui->playlistComboBox->count()) % ui->playlistComboBox->count();
    ui->playlistComboBox->setCurrentIndex(prevIndex);
}

void MainWindow::on_volumeSlider_valueChanged(int value)
{
    player->setVolume(value);
    ui->volumeLabel->setText("Громкость: " + QString::number(value) + "%");
}

void MainWindow::updatePlayerPosition(qint64 position)
{
    ui->progressBar->setValue(position);
}

void MainWindow::updatePlayerDuration(qint64 duration)
{
    ui->progressBar->setMaximum(duration);
    ui->songLabel->setText("Играет: " + ui->playlistComboBox->currentText());
}

void MainWindow::on_playlistComboBox_currentIndexChanged(int index)
{
    ui->songLabel->setText("Играет: " + ui->playlistComboBox->itemText(index));
    player->play();
    ui->playButton->setText("⏸");
}

// Новый метод для установки случайного времени
void MainWindow::on_btnRandomTime_clicked()
{
    QTime randomTime = QTime::currentTime().addSecs(QRandomGenerator::global()->bounded(3600)); // +0-60 минут
    ui->timeCurtain->setTime(randomTime);
    ui->timeKettle->setTime(randomTime);
    ui->timeLight->setTime(randomTime);
    ui->timeSocket->setTime(randomTime);
    QMessageBox::information(this, "Случайное время", "Установлено время: " + randomTime.toString("HH:mm"));
}
