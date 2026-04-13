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

    m_pWidgetManger = new WidgetManger(this);
    ui->verticalLayout_2->addWidget(m_pWidgetManger);
    // 重置窗口大小
    resize(WINDOW_WIDTH, WINDOW_HEIGHT);

    m_nTimer1 = startTimer(200);
    m_nTimer2 = startTimer(20);
    InitStatusBar(ui->statusbar);
    RegistStruct();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::timerEvent(QTimerEvent *event)
{
    if (event->timerId() == m_nTimer1)
    {
        BaseWidget *baseWidget = dynamic_cast<BaseWidget *>(m_pWidgetManger->GetCurrentWidget());
        if (baseWidget != NULL)
        {
            baseWidget->MessageQueue(MsgData::REFRESH, "");
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
