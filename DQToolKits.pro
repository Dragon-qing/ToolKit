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
    src src/core\
    src/widget\
    src/model src/service\
    src/thirdparty src/thirdparty/easyqtsql\
    src/widget/components\
    src/utils\
INCLUDEPATH += $$PWD/include
LIBS += -luser32
CONFIG(debug, debug|release) {
    LIBS += -L$$PWD/lib/debug -lqhotkey
    LIBS += -L$$PWD/lib/debug -lspdlogd
    LIBS += -L$$PWD/lib/debug -lfmtd
} else {
    LIBS += -L$$PWD/lib -lqhotkey
    LIBS += -L$$PWD/lib -lspdlog
    LIBS += -L$$PWD/lib -lfmt
}

SOURCES += \
    src/main.cpp \
    src/mainwindow.cpp \
    # core
    src/core/basewidget.cpp \
    src/core/sysapi.cpp \
    src/core/widgetmanger.cpp\
    # 数据层
    src/model/datacomrpt.cpp \
    src/model/dataconfig.cpp \
    src/model/datalog.cpp \
    src/model/datapainter.cpp \
    src/model/dataxmleditor.cpp \
    src/model/msgdata.cpp \
    # 服务层
    src/service/dataservice.cpp \
    # 组件
    src/widget/components/basetable.cpp \
    src/widget/components/unionplot.cpp \
    # 界面
    src/widget/dlgabout.cpp \
    src/widget/dlgbtfmakeinfo.cpp \
    src/widget/dlgbtfprocess.cpp \
    src/widget/dlgcomres.cpp \
    src/widget/dlghelp.cpp \
    src/widget/dlgprompt.cpp \
    src/widget/dlgunionplotsubline.cpp \
    src/widget/wgbtfmake.cpp \
    src/widget/wgcomrpt.cpp \
    src/widget/wglog.cpp \
    src/widget/wgrenametool.cpp \
    src/widget/wgtyproaimgcleaner.cpp \
    src/widget/wgxmleditor.cpp \
    # 工具类
    src/utils/tklogger.cpp \
    src/utils/common.cpp \
    # QCP第三方绘图模块引入
    src/thirdparty/qcustomplot.cpp

HEADERS += \
    src/mainwindow.h \
    # core
    src/core/basewidget.h \
    src/core/datadef.h \
    src/core/sysapi.h \
    src/core/widgetmanger.h\
    # 数据层
    src/model/datacomrpt.h \
    src/model/dataconfig.h \
    src/model/datalog.h \
    src/model/datapainter.h \
    src/model/dataxmleditor.h \
    src/model/msgdata.h \
    # 服务层
    src/service/dataservice.h \
    # 组件
    src/widget/components/basetable.h \
    src/widget/components/unionplot.h \
    # 界面
    src/widget/dlgabout.h \
    src/widget/dlgbtfmakeinfo.h \
    src/widget/dlgbtfprocess.h \
    src/widget/dlgcomres.h \
    src/widget/dlghelp.h \
    src/widget/dlgprompt.h \
    src/widget/dlgunionplotsubline.h \
    src/widget/wgbtfmake.h \
    src/widget/wgcomrpt.h \
    src/widget/wglog.h \
    src/widget/wgrenametool.h \
    src/widget/wgtyproaimgcleaner.h \
    src/widget/wgxmleditor.h \
    # 工具类
    src/utils/tklogger.h \
    src/utils/common.h \
    # QCP第三方绘图模块引入
    src/thirdparty/qcustomplot.h \

FORMS += \
    src/mainwindow.ui \
    # core
    src/core/basewidget.ui \
    src/core/widgetmanger.ui\
    # 数据层
    src/model/datapainter.ui \
    # 组件
    src/widget/components/basetable.ui \
    src/widget/components/unionplot.ui \
    # 界面层
    src/widget/dlgabout.ui \
    src/widget/dlgbtfmakeinfo.ui \
    src/widget/dlgbtfprocess.ui \
    src/widget/dlgcomres.ui \
    src/widget/dlghelp.ui \
    src/widget/dlgprompt.ui \
    src/widget/dlgunionplotsubline.ui \
    src/widget/wgbtfmake.ui \
    src/widget/wgcomrpt.ui \
    src/widget/wglog.ui \
    src/widget/wgrenametool.ui \
    src/widget/wgtyproaimgcleaner.ui \
    src/widget/wgxmleditor.ui \

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    res.qrc
