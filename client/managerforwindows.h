#ifndef MANAGERFORWINDOWS_H
#define MANAGERFORWINDOWS_H

#include <QObject>
#include "smarthomewindow.h"
#include "mainwindow.h"
#include "registerdialog.h"

class ManagerForWindows : public QObject
{
    Q_OBJECT
public:
    explicit ManagerForWindows(QObject *parent = nullptr);
    void showAuthWindow();
    void showMainWindow();
    void showRegWindow();

private:
    SmarthomeWindow* m_authWindow;
    MainWindow* m_mainWindow;
    RegisterDialog* m_regWindow;
};

#endif // MANAGERFORWINDOWS_H
