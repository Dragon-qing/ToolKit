/*
 * @Author: dragon-qing
 * @Date: 2025-04-25 10:28:24
 * @LastEditors: Dragon-qing
 * @LastEditTime: 2026-04-18
 * @FilePath: \ToolKit\src\ui\components\basewidget.cpp
 * @Description: 
 */
#include "basewidget.h"
#include "ui_basewidget.h"

BaseWidget::BaseWidget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::BaseWidget)
{
    ui->setupUi(this);
}

BaseWidget::~BaseWidget()
{
    delete ui;
}

/**
 * @description: 消息处理接口
 * @param {QVariant} messageid
 * @param {QVariant} messageValue
 * @return
 */
void BaseWidget::MessageFlows(QVariant messageid, QVariant messageValue)
{
    Q_UNUSED(messageid);
    Q_UNUSED(messageValue);

    return;
}

/**
 * @description: 帮助文档信息
 * @return {*}
 */
QStringList BaseWidget::GetHelpText()
{
    return QStringList();
}
