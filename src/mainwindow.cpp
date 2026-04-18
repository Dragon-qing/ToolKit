#include <QSize>

#include "basewidget.h"
#include "datapainter.h"
#include "sysapi.h"
#include "datacomrpt.h"

#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setStyleSheet(DataPainter::GetInstance().GetStyleSheetStr()); // 设置样式表

    m_pWidgetManager = new WidgetManager(this);
    ui->verticalLayout_2->addWidget(m_pWidgetManager);
    // 重置窗口大小
    resize(WINDOW_WIDTH, WINDOW_HEIGHT);

    m_nTimer1 = startTimer(200);
    m_nTimer2 = startTimer(20);
    InitStatusBar(ui->statusbar);
    RegistStruct();
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
