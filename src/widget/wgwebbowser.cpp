/*
 * @Author: dragon-qing
 * @Date: 2025-05-30 13:48:08
 * @LastEditors: dragon-qing
 * @LastEditTime: 2025-05-30 14:18:55
 * @FilePath: \ToolKit\src\widget\wgwebbowser.cpp
 * @Description: 内置浏览器界面
 */

#include "wgwebbowser.h"
#include "ui_wgwebbowser.h"

WgWebBowser::WgWebBowser(QWidget *parent)
    : BaseWidget(parent)
    , ui(new Ui::WgWebBowser)
{
    ui->setupUi(this);
    m_pWebView = new TKWebView(this);
    ui->verticalLayout->addWidget(m_pWebView);
    m_pWebView->load(QUrl("https://www.google.com"));
    // view.show();
}

WgWebBowser::~WgWebBowser()
{
    delete ui;
}
