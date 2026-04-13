QT       += core gui sql
QT       += xml


greaterThan(QT_MAJOR_VERSION, 4): QT += widgets printsupport

CONFIG += c++20
CONFIG -= depend_includepath # 去除路径依赖

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0
DESTDIR = $$PWD/bin
OBJECTS_DIR = $$OUT_PWD/obj
MOC_DIR = $$OUT_PWD/moc
UI_DIR = $$OUT_PWD/ui
RCC_DIR = $$OUT_PWD/rcc

# 引入EasyQtSql第三方sql库
include(src/thirdparty/easyqtsql/EasyQtSql.pri)

msvc {
    QMAKE_CFLAGS += /utf-8
    QMAKE_CXXFLAGS += /utf-8
}
RC_ICONS = img/toolbox.ico
INCLUDEPATH += \
    src src/widget\
    src/data src/service\
    src/thirdparty src/thirdparty/easyqtsql\
    src/widget/control\
    src/utils\
INCLUDEPATH += $$PWD/include
LIBS += -luser32
CONFIG(debug, debug|release) {
    LIBS += -L$$PWD/lib/debug -lqhotkey
    LIBS += -L$$PWD/lib/debug -lspdlogd
} else {
    LIBS += -L$$PWD/lib -lqhotkey
    LIBS += -L$$PWD/lib -lspdlog
}

SOURCES += \
    src/basewidget.cpp \
    src/common.cpp \
    src/main.cpp \
    src/mainwindow.cpp \
    src/msgdata.cpp \
    # 数据层
    src/data/datacomrpt.cpp \
    src/data/dataconfig.cpp \
    src/data/datalog.cpp \
    src/data/datapainter.cpp \
    src/data/dataxmleditor.cpp \
    src/data/sysapi.cpp \
    # 服务层
    src/service/dataservice.cpp \
    # 界面
    src/widget/control/basetable.cpp \
    src/widget/dlgabout.cpp \
    src/widget/dlgbtfmakeinfo.cpp \
    src/widget/dlgbtfprocess.cpp \
    src/widget/dlgcomres.cpp \
    src/widget/dlghelp.cpp \
    src/widget/dlgprompt.cpp \
    src/widget/dlgunionplotsubline.cpp \
    src/widget/unionplot.cpp \
    src/widget/wgbtfmake.cpp \
    src/widget/wgcomrpt.cpp \
    src/widget/wglog.cpp \
    src/widget/wgrenametool.cpp \
    src/widget/wgtyproaimgcleaner.cpp \
    src/widget/wgxmleditor.cpp \
    src/widget/widgetmanger.cpp\
    # 工具类
    src/utils/tklogger.cpp \
    # QCP第三方绘图模块引入
    src/thirdparty/qcustomplot.cpp

HEADERS += \
    src/basewidget.h \
    src/common.h \
    src/datadef.h \
    src/mainwindow.h \
    src/msgdata.h \
    # 数据层
    src/data/datacomrpt.h \
    src/data/dataconfig.h \
    src/data/datalog.h \
    src/data/datapainter.h \
    src/data/dataxmleditor.h \
    src/data/sysapi.h \
    # 服务层
    src/service/dataservice.h \
    # 界面
    src/widget/control/basetable.h \
    src/widget/dlgabout.h \
    src/widget/dlgbtfmakeinfo.h \
    src/widget/dlgbtfprocess.h \
    src/widget/dlgcomres.h \
    src/widget/dlghelp.h \
    src/widget/dlgprompt.h \
    src/widget/dlgunionplotsubline.h \
    src/widget/unionplot.h \
    src/widget/wgbtfmake.h \
    src/widget/wgcomrpt.h \
    src/widget/wglog.h \
    src/widget/wgrenametool.h \
    src/widget/wgtyproaimgcleaner.h \
    src/widget/wgxmleditor.h \
    src/widget/widgetmanger.h\
    # 工具类
    src/utils/tklogger.h \
    # QCP第三方绘图模块引入
    src/thirdparty/qcustomplot.h

FORMS += \
    src/basewidget.ui \
    src/data/datapainter.ui \
    src/mainwindow.ui \
    src/widget/control/basetable.ui \
    src/widget/dlgabout.ui \
    src/widget/dlgbtfmakeinfo.ui \
    src/widget/dlgbtfprocess.ui \
    src/widget/dlgcomres.ui \
    src/widget/dlghelp.ui \
    src/widget/dlgprompt.ui \
    src/widget/dlgunionplotsubline.ui \
    src/widget/unionplot.ui \
    src/widget/wgbtfmake.ui \
    src/widget/wgcomrpt.ui \
    src/widget/wglog.ui \
    src/widget/wgrenametool.ui \
    src/widget/wgtyproaimgcleaner.ui \
    src/widget/wgxmleditor.ui \
    src/widget/widgetmanger.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    res.qrc
