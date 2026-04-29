/*
 * @Author: Dragon-qing
 * @Date: 2026-04-29
 * @LastEditors: Dragon-qing
 * @FilePath: \ToolKit\src\ui\uiregister.h
 * @Description: ui注册头文件，定义注册宏
 */
#ifndef UIREGISTER_H
#define UIREGISTER_H

#include "uifactory.h"
#define REGISTER_UI_PAGE(KEY, MENU, TEXT, TYPE, ORDER)                          \
    UIFactory::Instance().RegisterPage(                                         \
        KEY, MENU, TEXT, [](QWidget* p){ return new TYPE(p); }, ORDER);         \

#define REGISTER_UI_DIALOG(KEY, MENU, TEXT, TYPE, ORDER)                        \
    UIFactory::Instance().RegisterDialog(                                       \
        KEY, MENU, TEXT, [](QWidget* p){ return new TYPE(p); }, ORDER);         \

#endif