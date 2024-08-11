QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0
OBJECTS_DIR = ../obj
MOC_DIR = ../moc
UI_DIR = ../ui
DESTDIR = ../bin

msvc {
    QMAKE_CFLAGS += /utf-8
    QMAKE_CXXFLAGS += /utf-8
}
INCLUDEPATH += \
    ../src ../src/widget\
LIBS += -luser32
SOURCES += \
    ../src/common.cpp \
    ../src/logdt.cpp \
    ../src/main.cpp \
    ../src/mainwindow.cpp \
    ../src/widget/wgbtfmake.cpp \
    ../src/widget/wggeneratebugfolder.cpp \
    ../src/widget/wgtyproaimgcleaner.cpp \
    ../src/widgetmanger.cpp

HEADERS += \
    ../src/common.h \
    ../src/datadef.h \
    ../src/logdt.h \
    ../src/mainwindow.h \
    ../src/widget/wgbtfmake.h \
    ../src/widget/wggeneratebugfolder.h \
    ../src/widget/wgtyproaimgcleaner.h \
    ../src/widgetmanger.h

FORMS += \
    ../src/mainwindow.ui \
    ../src/widget/wgbtfmake.ui \
    ../src/widget/wggeneratebugfolder.ui \
    ../src/widget/wgtyproaimgcleaner.ui \
    ../src/widgetmanger.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    ../res.qrc
