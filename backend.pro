QT += core network sql
CONFIG += c++17
TARGET = SmartHomeServer
CONFIG += console
CONFIG -= app_bundle

SOURCES += \
    clienthandler.cpp \
    main.cpp \
    tcp_server.cpp

HEADERS += \
    DatabaseSingleton.h \
    clienthandler.h \
    tcp_server.h
