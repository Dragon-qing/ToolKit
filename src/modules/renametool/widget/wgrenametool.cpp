/**
 * @brief 重命名工具界面
 * @file wgrenametool.cpp
 * @date 2026/01/22
 * @author Dragonqing
 */

#include "wgrenametool.h"
#include "ui_wgrenametool.h"

WgRenameTool::WgRenameTool(QWidget *parent)
    : BaseWidget(parent)
    , ui(new Ui::WgRenameTool)
{
    ui->setupUi(this);
}

WgRenameTool::~WgRenameTool()
{
    delete ui;
}


void WgRenameTool::MessageFlows(QVariant messageid, QVariant messageValue)
{
}

QStringList WgRenameTool::GetHelpText()
{
}