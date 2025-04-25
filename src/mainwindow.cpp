#include <QSize>

#include "basewidget.h"
#include "hmipainter.h"

#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setStyleSheet(HmiPainter::GetInstance().GetStyleSheetStr()); // 设置样式表

    m_pWidgetManger = new WidgetManger(this);
    ui->verticalLayout_2->addWidget(m_pWidgetManger);
    // 重置窗口大小
    resize(WINDOW_WIDTH, WINDOW_HEIGHT);

    m_nTimer1 = startTimer(200);
    m_nTimer2 = startTimer(20);
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
            baseWidget->MassageQueue(MsgData::REFRESH, "");
        }
    }
    else if (event->timerId() == m_nTimer2)
    {

    }
}

