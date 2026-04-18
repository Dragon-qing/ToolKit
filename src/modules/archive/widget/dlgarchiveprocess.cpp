/*!
 * @brief 压缩包制作过程弹窗
 * @file dlgarchiveprocess.cpp
 * @author Dragon_qing
 * @date 2025/05/23
 */

#include "dlgarchiveprocess.h"
#include "ui_dlgarchiveprocess.h"

#include <QPainter>
#include <QPainterPath>
#include <QStyleOption>
#include <QMouseEvent>

#include "common.h"

DlgArchiveProcess::DlgArchiveProcess(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::DlgArchiveProcess)
    , m_pExternalTool(nullptr)
{
    ui->setupUi(this);
    setWindowFlag(Qt::FramelessWindowHint, true);
    setAttribute(Qt::WA_TranslucentBackground); // 允许透明背景
    m_timer = new QTimer(this);
    connect(m_timer, &QTimer::timeout, this, &DlgArchiveProcess::TimeOutHandle);
    setMinimumSize(DLG_WIDTH, DLG_HEIGHT);
    adjustSize();
}

DlgArchiveProcess::~DlgArchiveProcess()
{
    delete ui;
}

void DlgArchiveProcess::paintEvent(QPaintEvent *event)
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

void DlgArchiveProcess::ReSet()
{
    m_timer->start(500);
    ui->label->setText(TR("正在制作压缩包"));
    ui->okBtn->setVisible(false); // 影藏按钮
    ui->cancelBtn->setVisible(true);
}

void DlgArchiveProcess::SetExternalTool(ExternalToolBase *pExternalTool)
{
    m_pExternalTool = pExternalTool;
}

void DlgArchiveProcess::Done()
{
    m_timer->stop();
    ui->label->setText(TR("制作完成！！！"));
    ui->okBtn->setVisible(true); // 显示按钮
    ui->cancelBtn->setVisible(false); // 影藏终止按钮
}

void DlgArchiveProcess::Faild()
{
    m_timer->stop();
    ui->label->setText(TR("制作失败"));
    ui->okBtn->setVisible(true);
    ui->cancelBtn->setVisible(false);
}

void DlgArchiveProcess::OnValueChanged(int value)
{
    ui->progressBar->setValue(value);
}

void DlgArchiveProcess::TimeOutHandle()
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

void DlgArchiveProcess::on_okBtn_clicked()
{
    this->close();
}

void DlgArchiveProcess::on_cancelBtn_clicked()
{
    if (m_pExternalTool)
    {
        m_pExternalTool->Stop();
    }
}
