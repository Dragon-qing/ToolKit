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
    src \
    src/core src/core/system \
    src/core/model src/core/logging \
    src/core/externaltool \
    src/core/config \
    src/core/threadmanager \
    src/ui src/ui/base src/ui/components \
    src/utils \
    src/thirdparty src/thirdparty/EasyQtSql \
    include \
    src/modules/about/widget \
    src/modules/archive/widget \
    src/modules/comrpt/widget \
    src/modules/comrpt/model \
    src/modules/help/widget \
    src/modules/logviewer/widget \
    src/modules/logviewer/model \
    src/modules/renametool/widget \
    src/modules/renametool/model \
    src/modules/renametool/service \
    src/modules/thememanager/model \
    src/modules/typroacleaner/widget \
    src/modules/xmleditor/model \
    src/modules/xmleditor/widget \

LIBS += -luser32
CONFIG(debug, debug|release) {
    LIBS += -L$$PWD/lib/debug -lqhotkey
    LIBS += -L$$PWD/lib/debug -lspdlogd
} else {
    LIBS += -L$$PWD/lib -lqhotkey
    LIBS += -L$$PWD/lib -lspdlog
}

SOURCES += \
    ./src/main.cpp  \
    ./src/mainwindow.cpp \
    # core
    ./src/core/system/sysapi.cpp \
    ./src/core/model/msgdata.cpp \
    ./src/core/logging/tklogger.cpp \
    ./src/core/config/configsetting.cpp \
    ./src/core/externaltool/externaltoolbase.cpp \
    ./src/core/externaltool/exeexternaltool.cpp \
    ./src/core/externaltool/shellexternaltool.cpp \
    ./src/core/externaltool/apiexternaltool.cpp \
    ./src/core/externaltool/toolfactory.cpp \
    ./src/core/threadmanager/threadmanager.cpp \
    # ui
    ./src/ui/base/basewidget.cpp \
    ./src/ui/components/basetable.cpp \
    ./src/ui/components/unionplot.cpp \
    ./src/ui/components/dlgprompt.cpp \
    ./src/ui/components/flowlayout.cpp \
    ./src/ui/widgetmanager.cpp \
    # 工具
    ./src/utils/common.cpp \
    ./src/utils/xmlopt.cpp \
    ./src/utils/stringopt.cpp \
    # 第三方
    ./src/thirdparty/qcustomplot.cpp \
    # modules
    # about模块
    ./src/modules/about/widget/dlgabout.cpp \
    # archive模块
    ./src/modules/archive/widget/dlgarchivemakeinfo.cpp \
    ./src/modules/archive/widget/dlgarchiveprocess.cpp \
    ./src/modules/archive/widget/wgarchivemaker.cpp \
    # comrpt模块
    ./src/modules/comrpt/model/datacomrpt.cpp \
    ./src/modules/comrpt/widget/dlgcomres.cpp \
    ./src/modules/comrpt/widget/dlgunionplotsubline.cpp \
    ./src/modules/comrpt/widget/wgcomrpt.cpp \
    # help模块
    ./src/modules/help/widget/dlghelp.cpp \
    # logviewer模块
    ./src/modules/logviewer/model/datalog.cpp \
    ./src/modules/logviewer/widget/wglog.cpp \
    # renametool模块
    ./src/modules/renametool/widget/wgrenametool.cpp \
    ./src/modules/renametool/service/renametoolservice.cpp \
    # typroacleaner模块
    ./src/modules/typroacleaner/widget/wgtyproaimgcleaner.cpp \
    # xmleditor模块
    ./src/modules/xmleditor/model/dataxmleditor.cpp \
    ./src/modules/xmleditor/widget/wgxmleditor.cpp \
    # thememanager模块
    ./src/modules/thememanager/model/datapainter.cpp

HEADERS += \
    ./src/mainwindow.h \
    # core
    ./src/core/system/sysapi.h \
    ./src/core/model/msgdata.h \
    ./src/core/model/datadef.h \
    ./src/core/logging/tklogger.h \
    ./src/core/config/configsetting.h \
    ./src/core/externaltool/externaltoolbase.h \
    ./src/core/externaltool/exeexternaltool.h \
    ./src/core/externaltool/shellexternaltool.h \
    ./src/core/externaltool/apiexternaltool.h \
    ./src/core/externaltool/toolfactory.h \
    ./src/core/externaltool/factoryregister.h \
    ./src/core/threadmanager/threadmanager.cpp \
    # ui
    ./src/ui/base/basewidget.h \
    ./src/ui/components/basetable.h \
    ./src/ui/components/unionplot.h \
    ./src/ui/components/dlgprompt.h \
    ./src/ui/components/flowlayout.h \
    ./src/ui/widgetmanager.h \
    # 工具
    ./src/utils/common.h \
    ./src/utils/xmlopt.h \
    ./src/utils/stringopt.h \
    # 第三方
    ./src/thirdparty/qcustomplot.h \
    # modules
    # about模块
    ./src/modules/about/widget/dlgabout.h \
    # archive模块
    ./src/modules/archive/widget/dlgarchivemakeinfo.h \
    ./src/modules/archive/widget/dlgarchiveprocess.h \
    ./src/modules/archive/widget/wgarchivemaker.h \
    # comrpt模块
    ./src/modules/comrpt/model/datacomrpt.h \
    ./src/modules/comrpt/widget/dlgcomres.h \
    ./src/modules/comrpt/widget/dlgunionplotsubline.h \
    ./src/modules/comrpt/widget/wgcomrpt.h \
    # help模块
    ./src/modules/help/widget/dlghelp.h \
    # logviewer模块
    ./src/modules/logviewer/model/datalog.h \
    ./src/modules/logviewer/widget/wglog.h \
    # renametool模块
    ./src/modules/renametool/widget/wgrenametool.h \
    ./src/modules/renametool/model/renametooldata.h \
    ./src/modules/renametool/service/renametoolservice.h \
    # typroacleaner模块
    ./src/modules/typroacleaner/widget/wgtyproaimgcleaner.h \
    # xmleditor模块
    ./src/modules/xmleditor/model/dataxmleditor.h \
    ./src/modules/xmleditor/widget/wgxmleditor.h \
    # thememanager模块
    ./src/modules/thememanager/model/datapainter.h

FORMS += \
    ./src/mainwindow.ui \
    # ui
    ./src/ui/base/basewidget.ui \
    ./src/ui/components/basetable.ui \
    ./src/ui/components/unionplot.ui \
    ./src/ui/components/dlgprompt.ui \
    ./src/ui/widgetmanager.ui \
    # modules
    # about模块
    ./src/modules/about/widget/dlgabout.ui \
    # archive模块
    ./src/modules/archive/widget/dlgarchivemakeinfo.ui \
    ./src/modules/archive/widget/dlgarchiveprocess.ui \
    ./src/modules/archive/widget/wgarchivemaker.ui \
    # comrpt模块
    ./src/modules/comrpt/widget/dlgcomres.ui \
    ./src/modules/comrpt/widget/dlgunionplotsubline.ui \
    ./src/modules/comrpt/widget/wgcomrpt.ui \
    # help模块
    ./src/modules/help/widget/dlghelp.ui \
    # logviewer模块
    ./src/modules/logviewer/widget/wglog.ui \
    # renametool模块
    ./src/modules/renametool/widget/wgrenametool.ui \
    # thememanager模块
    ./src/modules/thememanager/model/datapainter.ui \
    # typroacleaner模块
    ./src/modules/typroacleaner/widget/wgtyproaimgcleaner.ui \
    # xmleditor模块
    ./src/modules/xmleditor/widget/wgxmleditor.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    res.qrc
