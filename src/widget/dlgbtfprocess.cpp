/*!
 * @brief BTF包制作弹窗
 * @file dlgbtfprocess.cpp
 * @author Dragon_qing
 * @date 2025/05/23
 */

#include "dlgbtfprocess.h"
#include "ui_dlgbtfprocess.h"

#include <QPainter>
#include <QPainterPath>
#include <QStyleOption>
#include <QMouseEvent>

#include "common.h"

DlgBtfProcess::DlgBtfProcess(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::DlgBtfProcess)
{
    ui->setupUi(this);
    setWindowFlag(Qt::FramelessWindowHint, true);
    setAttribute(Qt::WA_TranslucentBackground); // 允许透明背景
    m_timer = new QTimer(this);
    connect(m_timer, &QTimer::timeout, this, &DlgBtfProcess::TimeOutHandle);
}

DlgBtfProcess::~DlgBtfProcess()
{
    delete ui;
}

void DlgBtfProcess::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing); // 开启抗锯齿
    QPainterPath path;
    path.addRoundedRect(rect(), 35, 35); // 设置圆角半径

    painter.setClipPath(path);
    painter.fillPath(path, Qt::white);

    QStyleOption opt;
    opt.init(this);
    style()->drawPrimitive(QStyle::PE_Widget, &opt, &painter, this);

    QDialog::paintEvent(event);
}

void DlgBtfProcess::Start()
{
    m_timer->start(500);
    ui->label->setText(TR("正在制作BTF包"));
    ui->widget->setVisible(false); // 影藏按钮
}

void DlgBtfProcess::Done()
{
    m_timer->stop();
    ui->label->setText(TR("制作完成！！！"));
    ui->widget->setVisible(true); // 显示按钮
}

void DlgBtfProcess::Faild()
{
    m_timer->stop();
    ui->label->setText(TR("制作失败"));
    ui->widget->setVisible(true);
}

void DlgBtfProcess::TimeOutHandle()
{
    QString text = ui->label->text();
    if (text.endsWith("..."))
    {
        text.replace(".", "");
    }
    else
    {
        text += ".";
    }
    ui->label->setText(text);
}

void DlgBtfProcess::on_okBtn_clicked()
{
    this->close();
}

