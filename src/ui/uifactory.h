/*
 * @Author: Dragon-qing
 * @Date: 2026-04-29
 * @LastEditors: Dragon-qing
 * @FilePath: \ToolKit\src\ui\uifactory.h
 * @Description: UI工厂类，负责创建界面
 */
#ifndef UIFACTORY_H
#define UIFACTORY_H

#include <QString>
#include <QWidget>

#include "datadef.h"

enum class UIMenuType { // 菜单分类
    Tools = 0, 
    Other, 
    Help 
};
enum class UIItemType { // UI项类型
    Page = 0, 
    Dialog 
};

struct UIEntry {
    UIItemType type;
    UIMenuType menu;
    QString key;
    QString actionText;
    Bit32 order = 0; // 菜单内排序，越小越靠前
};

class UIFactory
{
public:
    using PageCreator = std::function<QWidget*(QWidget*)>;
    using DialogCreator = std::function<QDialog*(QWidget*)>;

    static UIFactory& Instance();
    ~UIFactory();

    /**
     * @brief: 注册页面创建函数
     * @note: key和text会被自动去除前后空格，且key在页面和对话框注册表中必须全局唯一
     * @param {QString&} key: 唯一标识符，用于创建页面
     * @param {UIMenuType} menu: 菜单类型
     * @param {QString&} text: 按钮显示文本
     * @param {PageCreator} c: 页面创建函数
     * @return {bool} 是否注册成功
     */
    bool RegisterPage(const QString& key, UIMenuType menu, const QString& text, PageCreator c, Bit32 order = 0);
    /**
     * @brief: 注册对话框创建函数
     * @note: key和text会被自动去除前后空格，且key在页面和对话框注册表中必须全局唯一
     * @param {QString&} key: 唯一标识符，用于创建对话框
     * @param {UIMenuType} menu: 菜单类型
     * @param {QString&} text: 按钮显示文本
     * @param {DialogCreator} c: 对话框创建函数
     * @return {bool} 是否注册成功
     */
    bool RegisterDialog(const QString& key, UIMenuType menu, const QString& text, DialogCreator c, Bit32 order = 0);

    QWidget* CreatePage(const QString& key, QWidget* parent) const;
    QDialog* CreateDialog(const QString& key, QWidget* parent) const;
    const QList<UIEntry>& Entries() const;

private:
    QHash<QString, PageCreator> m_pageCreators;
    QHash<QString, DialogCreator> m_dialogCreators;
    QList<UIEntry> m_entries;

    UIFactory();
};

#endif