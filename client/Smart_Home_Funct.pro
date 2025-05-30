QT += core gui widgets network
CONFIG += c++17
TARGET = SmartHome
TEMPLATE = app

SOURCES += \
    main.cpp \
    smart_home_client.cpp \
    managerforwindows.cpp \
    smarthomewindow.cpp \
    registerdialog.cpp \
    mainwindow.cpp \
    func_for_client.cpp

HEADERS += \
    smart_home_client.h \
    managerforwindows.h \
    smarthomewindow.h \
    registerdialog.h \
    mainwindow.h \
    func_for_client.h

FORMS += \
    smarthomewindow.ui \
    registerdialog.ui \
    mainwindow.ui

# Настройки для корректной генерации UI
CONFIG += uic

# Настройки для Windows
win32 {
    CONFIG += console
    # Правильный флаг для GCC/MinGW:
    QMAKE_CXXFLAGS += -fpermissive
    # Для MSVC используйте:
    # QMAKE_CXXFLAGS += /permissive
}

# Оптимизация сборки
CONFIG(release, debug|release) {
    DEFINES += QT_NO_DEBUG_OUTPUT
}
