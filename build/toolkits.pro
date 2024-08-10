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

INCLUDEPATH += \
    ../src ../src/widget\

SOURCES += \
    ../src/common.cpp \
    ../src/logdt.cpp \
    ../src/main.cpp \
    ../src/mainwindow.cpp \
    ../src/widget/wggeneratebugfolder.cpp \
    ../src/widget/wgtyproaimgcleaner.cpp

HEADERS += \
    ../src/common.h \
    ../src/datadef.h \
    ../src/logdt.h \
    ../src/mainwindow.h \
    ../src/widget/wggeneratebugfolder.h \
    ../src/widget/wgtyproaimgcleaner.h

FORMS += \
    ../src/mainwindow.ui \
    ../src/widget/wggeneratebugfolder.ui \
    ../src/widget/wgtyproaimgcleaner.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
