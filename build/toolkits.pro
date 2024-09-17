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
RC_ICONS = ../img/toolbox.ico
INCLUDEPATH += \
    ../src ../src/widget\
LIBS += -luser32
SOURCES += \
    ../src/common.cpp \
    ../src/hmiconfig.cpp \
    ../src/logdt.cpp \
    ../src/main.cpp \
    ../src/mainwindow.cpp \
    ../src/widget/wgbtfmake.cpp \
    ../src/widget/wggeneratebugfolder.cpp \
    ../src/widget/wgtestpython.cpp \
    ../src/widget/wgtyproaimgcleaner.cpp \
    ../src/widgetmanger.cpp

HEADERS += \
    ../src/common.h \
    ../src/datadef.h \
    ../src/hmiconfig.h \
    ../src/logdt.h \
    ../src/mainwindow.h \
    ../src/widget/wgbtfmake.h \
    ../src/widget/wggeneratebugfolder.h \
    ../src/widget/wgtestpython.h \
    ../src/widget/wgtyproaimgcleaner.h \
    ../src/widgetmanger.h

FORMS += \
    ../src/mainwindow.ui \
    ../src/widget/wgbtfmake.ui \
    ../src/widget/wggeneratebugfolder.ui \
    ../src/widget/wgtestpython.ui \
    ../src/widget/wgtyproaimgcleaner.ui \
    ../src/widgetmanger.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    ../res.qrc

# python environment config

win32: LIBS += -L$$PWD/'../../../Program Files/python312/libs/' -lpython312

INCLUDEPATH += $$PWD/'../../../Program Files/python312/include'
DEPENDPATH += $$PWD/'../../../Program Files/python312/include'

win32:!win32-g++: PRE_TARGETDEPS += $$PWD/'../../../Program Files/python312/libs/python312.lib'
