#-------------------------------------------------
#
# Project created by QtCreator 2018-11-06T10:19:08
#
#-------------------------------------------------

QT       += core gui charts quick opcua widgets

#greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = FChart
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

CONFIG += c++11

SOURCES += \
        main.cpp \
        mainwindow.cpp \
        src/information/CFrequencyface.cpp \
        src/chart/CChartWidget.cpp \
        src/FrequencyDisplay.cpp \
        src/components/WindowHead.cpp \
        src/components/ImageButton.cpp \
        src/backend/opcuamachinebackend.cpp \
        src/backend/processcontrol.cpp

HEADERS += \
        mainwindow.h \
        src/information/CFrequencyface.h \
        src/chart/CChartWidget.h \
        src/FrequencyDisplay.h \
        src/components/WindowHead.h \
        src/components/ImageButton.h \
        src/backend/opcuamachinebackend.h \
        src/backend/processcontrol.h

FORMS += \
        mainwindow.ui \
        src/information/CFrequencyface.ui \
        src/FrequencyDisplay.ui \
        src/components/WindowHead.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    res.qrc

include(xlsx/qtxlsx.pri)
RC_FILE += res.rc
