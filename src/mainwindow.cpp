#include <QSize>

#include "common.h"
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
}

MainWindow::~MainWindow()
{
    delete ui;
}

