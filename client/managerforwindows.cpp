#include "managerforwindows.h"
#include "smart_home_client.h"

ManagerForWindows::ManagerForWindows(QObject *parent) : QObject(parent)
{
    m_authWindow = new SmarthomeWindow();
    m_mainWindow = new MainWindow();
    m_regWindow = new RegisterDialog();

    // Подключение сигналов от окон
    connect(m_authWindow, &SmarthomeWindow::show_reg, this, &ManagerForWindows::showRegWindow);
    connect(m_authWindow, &SmarthomeWindow::login_success, this, &ManagerForWindows::showMainWindow);
    connect(m_regWindow, &RegisterDialog::reg_ok, this, &ManagerForWindows::showAuthWindow);
}

void ManagerForWindows::showAuthWindow()
{
    m_mainWindow->hide();
    m_regWindow->hide();
    m_authWindow->show();
}

void ManagerForWindows::showMainWindow()
{
    m_authWindow->hide();
    m_regWindow->hide();
    m_mainWindow->show();
}

void ManagerForWindows::showRegWindow()
{
    m_authWindow->hide();
    m_mainWindow->hide();
    m_regWindow->show();
}
