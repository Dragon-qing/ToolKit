#include <QSize>

#include "common.h"
#include "wggeneratebugfolder.h"
#include "wgtyproaimgcleaner.h"

#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    m_widgetContainer.clear();
    m_widgetContainer.append(new WgGenerateBugFolder());
    m_widgetContainer.append(new WgTyproaImgCleaner());

    m_nCurrentWidgetIdx = 0;
    // 重置窗口大小
    resize(WINDOW_WIDTH, WINDOW_HEIGHT);
    SetCurrentWidget(0);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::SetCurrentWidget(Bit32 idx)
{
    if (idx < 0 || idx >= m_widgetContainer.count())
    {
        LogDt::Instance().InsertLog(LogDt::SYSTEM_ERROR_LOG_TYPE, QObject::TR("窗口切换失败，索引超出范围"));
        return;
    }

    while(ui->verticalLayout_2->takeAt(0) != nullptr){}
    ui->verticalLayout_2->addWidget(m_widgetContainer.at(idx));
    m_nCurrentWidgetIdx = idx;
}

void MainWindow::InerstWidget(QWidget *widget, QAction *action, Bit32 widgetType)
{
    if (widget == nullptr || action == nullptr)
    {
        return;
    }

    m_widgetContainer.append(widget);
}

