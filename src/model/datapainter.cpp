/*!
 * @brief 样式管理接口
 * @note 用于获取各类样式颜色等。
 * @file datapainter.cpp
 * @author Dragon_qing
 * @date 2025/04/15
 */

#include <QFileInfo>

#include "datapainter.h"
#include "ui_datapainter.h"

#define STYLESHEET_FILE "style.qss"

DataPainter::DataPainter(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::DataPainter)
{
    ui->setupUi(this);

    Init();
}

DataPainter &DataPainter::GetInstance()
{
    static DataPainter *s_data = NULL;
    if (s_data == NULL)
    {
        s_data = new DataPainter();
    }

    return *s_data;
}

DataPainter::~DataPainter()
{
    delete ui;
}

void DataPainter::Init()
{
    // 获取qss文件样式
    QFile qssFile(QString(":/style/%1").arg(STYLESHEET_FILE));
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
