/*!
 * @brief 样式管理接口
 * @note 用于获取各类样式颜色等。
 * @file hmipainter.cpp
 * @author Dragon_qing
 * @date 2025/04/15
 */

#include <QFileInfo>

#include "hmipainter.h"
#include "ui_hmipainter.h"

#define STYLESHEET_FILE "style.qss"

HmiPainter::HmiPainter(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::HmiPainter)
{
    ui->setupUi(this);

    Init();
}

HmiPainter &HmiPainter::GetInstance()
{
    static HmiPainter *s_data = NULL;
    if (s_data == NULL)
    {
        s_data = new HmiPainter();
    }

    return *s_data;
}

HmiPainter::~HmiPainter()
{
    delete ui;
}

void HmiPainter::Init()
{
    // 获取qss文件样式
    QFile qssFile(STYLESHEET_FILE);
    if (qssFile.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        m_sStyleSheet = qssFile.readAll();
        qssFile.close();
    }
    else
    {
        // 输出错误日志
    }

}
