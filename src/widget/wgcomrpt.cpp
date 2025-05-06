/*!
 * @brief 调机数据分析页面
 * @file wgcomrpt.cpp
 * @author Dragon_qing
 * @date 2025/05/06
 */
#include "hmicomrpt.h"

#include "wgcomrpt.h"
#include "ui_wgcomrpt.h"

WgComRpt::WgComRpt(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::WgComRpt)
{
    ui->setupUi(this);
    HmiComRpt::Instance().SetPath("C:/Users/wan/Desktop/机床采样文件/MC_0_8000000001_AX_0");
}

WgComRpt::~WgComRpt()
{
    delete ui;
}
