QT += core gui widgets network
CONFIG += c++17
TARGET = clientV2
TEMPLATE = app

SOURCES += \
    main.cpp \
    smart_home_client.cpp \
    smarthomewindow.cpp \
    registerdialog.cpp \
    mainwindow.cpp

HEADERS += \
    registerdialog.h \
    smart_home_client.h \
    smarthomewindow.h \
    registerdialog.h \
    mainwindow.h

FORMS += \
    smarthomewindow.ui \
    registerdialog.ui \
    mainwindow.ui
