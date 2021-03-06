QT       += core gui
QT += network printsupport
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    dataio.cpp \
    main.cpp \
    mainwindow.cpp \
    qcustomplot.cpp \
    settings.cpp \
    signaldata.cpp \
    viewsetting.cpp

HEADERS += \
    dataio.h \
    mainwindow.h \
    qcustomplot.h \
    settings.h \
    signaldata.h \
    viewsetting.h

FORMS += \
    mainwindow.ui \
    viewsetting.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
