QT += core network sql testlib
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

CONFIG(BUILD_TESTS) {
    DEFINES += BUILD_TESTS
    SOURCES += test_database_singleton.cpp
    HEADERS += test_database_singleton.h
}
