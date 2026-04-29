#include <QSize>

#include "basewidget.h"
#include "common.h"
#include "datapainter.h"
#include "sysapi.h"
#include "datacomrpt.h"
#include "threadmanager.h"
#include "tklogger.h"
#include "uiregister.h"

#include "wgarchivemaker.h"
#include "dlgabout.h"
#include "wgxmleditor.h"
#include "wgcomrpt.h"
#include "wgrenametool.h"
#include "wglog.h"

#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setStyleSheet(DataPainter::GetInstance().GetStyleSheetStr()); // 设置样式表
    RegistUI(); // 注册UI
    m_pWidgetManager = new WidgetManager(this);
    ui->verticalLayout_2->addWidget(m_pWidgetManager);
    // 重置窗口大小
    resize(WINDOW_WIDTH, WINDOW_HEIGHT);

    m_nTimer1 = startTimer(200);
    m_nTimer2 = startTimer(20);
    InitStatusBar(ui->statusbar);
    RegistStruct();

    // 初始化线程池
    ThreadManager::Instance();
}

MainWindow::~MainWindow()
{
    TKLogger::Instance().AddLog(INFO_LOG, TR("主程序关闭"));
    delete ui;
    DestoryStatusBar();
}

void MainWindow::timerEvent(QTimerEvent *event)
{
    if (event->timerId() == m_nTimer1)
    {
        BaseWidget *baseWidget = dynamic_cast<BaseWidget *>(m_pWidgetManager->GetCurrentWidget());
        if (baseWidget != NULL)
        {
            baseWidget->MessageFlows(MsgData::REFRESH, "");
        }
    }
    else if (event->timerId() == m_nTimer2)
    {

    }
}

void MainWindow::RegistStruct()
{
    qRegisterMetaType<ComResult>("ComResult");
}

void MainWindow::RegistUI()
{
    // 确保UI只被注册一次
    static bool s_uiRegistered = false;
    if (s_uiRegistered)
    {
        return;
    }
    s_uiRegistered = true;

    // 工具菜单组
    REGISTER_UI_PAGE("wg_archivemaker", UIMenuType::Tools, QObject::TR("压缩包制作"), WgArchiveMaker, 0)
    REGISTER_UI_PAGE("wg_xmleditor", UIMenuType::Tools, QObject::TR("XML编辑器"), WgXmlEditor, 1)
    REGISTER_UI_PAGE("wg_comrpt", UIMenuType::Tools, QObject::TR("调机数据分析"), WgComRpt, 2)
    REGISTER_UI_PAGE("wg_renametool", UIMenuType::Tools, QObject::TR("文件重命名"), WgRenameTool, 3)

    // 帮助菜单组
    REGISTER_UI_PAGE("wg_log", UIMenuType::Help, QObject::TR("日志"), WgLog, 0)
    REGISTER_UI_DIALOG("dlg_about", UIMenuType::Help, QObject::TR("关于"), DlgAbout, 100)
}
