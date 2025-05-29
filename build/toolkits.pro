QT       += core gui sql
QT       += xml


greaterThan(QT_MAJOR_VERSION, 4): QT += widgets printsupport

CONFIG += c++11

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0
OBJECTS_DIR = ../obj
MOC_DIR = ../moc
UI_DIR = ../ui
DESTDIR = ../bin

# 引入EasyQtSql第三方sql库
include(../src/thirdparty/easyqtsql/EasyQtSql.pri)

msvc {
    QMAKE_CFLAGS += /utf-8
    QMAKE_CXXFLAGS += /utf-8
}
RC_ICONS = ../img/toolbox.ico
INCLUDEPATH += \
    ../src ../src/widget\
    ../src/hmi ../src/service\
    ../src/thirdparty ../src/thirdparty/easyqtsql\
    ../src/widget/control\
INCLUDEPATH += ../include
LIBS += -luser32
LIBS += -L$$PWD/../lib
LIBS += -lqhotkey

SOURCES += \
    ../src/basewidget.cpp \
    ../src/common.cpp \
    ../src/hmi/hmicomrpt.cpp \
    ../src/hmi/hmilog.cpp \
    ../src/hmi/sysapi.cpp \
    ../src/logdt.cpp \
    ../src/main.cpp \
    ../src/mainwindow.cpp \
    ../src/msgdata.cpp \
    ../src/service/dataservice.cpp \
    ../src/hmi/hmiconfig.cpp \
    ../src/hmi/hmipainter.cpp \
    ../src/hmi/hmixmleditor.cpp \
    ../src/widget/control/basetable.cpp \
    ../src/widget/dlgbtfmakeinfo.cpp \
    ../src/widget/dlgbtfprocess.cpp \
    ../src/widget/dlgcomres.cpp \
    ../src/widget/dlghelp.cpp \
    ../src/widget/dlgprompt.cpp \
    ../src/widget/dlgunionplotsubline.cpp \
    ../src/widget/unionplot.cpp \
    ../src/widget/wgbtfmake.cpp \
    ../src/widget/wgcomrpt.cpp \
    ../src/widget/wggeneratebugfolder.cpp \
    ../src/widget/wglog.cpp \
    ../src/widget/wgtyproaimgcleaner.cpp \
    ../src/widget/wgxmleditor.cpp \
    ../src/widget/widgetmanger.cpp\
    # QCP第三方绘图模块引入
    ../src/thirdparty/qcustomplot.cpp

HEADERS += \
    ../src/basewidget.h \
    ../src/common.h \
    ../src/datadef.h \
    ../src/hmi/hmicomrpt.h \
    ../src/hmi/hmilog.h \
    ../src/hmi/sysapi.h \
    ../src/logdt.h \
    ../src/mainwindow.h \
    ../src/msgdata.h \
    ../src/service/dataservice.h \
    ../src/hmi/hmiconfig.h \
    ../src/hmi/hmipainter.h \
    ../src/hmi/hmixmleditor.h \
    ../src/widget/control/basetable.h \
    ../src/widget/dlgbtfmakeinfo.h \
    ../src/widget/dlgbtfprocess.h \
    ../src/widget/dlgcomres.h \
    ../src/widget/dlghelp.h \
    ../src/widget/dlgprompt.h \
    ../src/widget/dlgunionplotsubline.h \
    ../src/widget/unionplot.h \
    ../src/widget/wgbtfmake.h \
    ../src/widget/wgcomrpt.h \
    ../src/widget/wggeneratebugfolder.h \
    ../src/widget/wglog.h \
    ../src/widget/wgtyproaimgcleaner.h \
    ../src/widget/wgxmleditor.h \
    ../src/widget/widgetmanger.h\
    # QCP第三方绘图模块引入
    ../src/thirdparty/qcustomplot.h

FORMS += \
    ../src/basewidget.ui \
    ../src/mainwindow.ui \
    ../src/hmi/hmipainter.ui \
    ../src/widget/control/basetable.ui \
    ../src/widget/dlgbtfmakeinfo.ui \
    ../src/widget/dlgbtfprocess.ui \
    ../src/widget/dlgcomres.ui \
    ../src/widget/dlghelp.ui \
    ../src/widget/dlgprompt.ui \
    ../src/widget/dlgunionplotsubline.ui \
    ../src/widget/unionplot.ui \
    ../src/widget/wgbtfmake.ui \
    ../src/widget/wgcomrpt.ui \
    ../src/widget/wggeneratebugfolder.ui \
    ../src/widget/wglog.ui \
    ../src/widget/wgtyproaimgcleaner.ui \
    ../src/widget/wgxmleditor.ui \
    ../src/widget/widgetmanger.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    ../res.qrc
