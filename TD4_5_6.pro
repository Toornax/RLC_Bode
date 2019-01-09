QT       += core gui widgets charts

TARGET = TD4_5_6
TEMPLATE = app

DEFINES += QT_DEPRECATED_WARNINGS

CONFIG += c++17

SOURCES += \
        main.cpp \
        MainWindow.cpp \
    CircuitDrawer.cpp \
    GraphDisplayGui.cpp

HEADERS += \
        MainWindow.hpp \
    CircuitDrawer.hpp \
    GraphDisplayGui.hpp

FORMS += \
        MainWindow.ui

RESOURCES += \
    res.qrc
