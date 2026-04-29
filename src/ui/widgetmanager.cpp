/*!
 * @brief 界面管理类
 * @file widgetmanager.cpp
 * @date 2024/08/11
 * @author Dragonqing
 */
#include <QMenuBar>
#include <algorithm>

#include "common.h"
#include "helpservice.h"
#include "uifactory.h"

#include "qhotkey.h"

#include "widgetmanager.h"
#include "ui_widgetmanager.h"

WidgetManager::WidgetManager(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::WidgetManager)
{
    ui->setupUi(this);
    m_widgetContainer.clear();
    m_dialogContainer.clear();
    m_pDlgHelp = new DlgHelp(this);
    m_menuList.clear();
    m_wgIdxMap.clear();
    m_dialogContainer.clear();

    InitTopMenu();
    InitWidgetContainer();

    for (Bit32 i = 0; i < m_widgetContainer.count(); i++)
    {
        QWidget *tmpWidget = m_widgetContainer.at(i);
        ui->stackedWidget->addWidget(tmpWidget);
    }

    connect(ui->stackedWidget, &QStackedWidget::currentChanged, this, &WidgetManager::WidgetChangeSlot);
    // 注册帮助页面全局快捷键
    QHotkey *helpHotKey = new QHotkey(QKeySequence("Ctrl+H"), true, qApp);
    connect(helpHotKey, &QHotkey::activated, this, &WidgetManager::CallHelpDlgSlot);
}

WidgetManager::~WidgetManager()
{
    delete ui;
}

QWidget *WidgetManager::GetCurrentWidget()
{
    Bit32 idx = ui->stackedWidget->currentIndex();

    if (idx < 0 || idx >= m_widgetContainer.count())
    {
        return NULL;
    }

    return m_widgetContainer.at(idx);
}

void WidgetManager::InitWidgetContainer()
{
    QList<UIEntry> entries = UIFactory::Instance().Entries();

    std::stable_sort(entries.begin(), entries.end(),
        [](const UIEntry& lhs, const UIEntry& rhs) {
            if (lhs.menu != rhs.menu)
            {
                return static_cast<int>(lhs.menu) < static_cast<int>(rhs.menu);
            }
            if (lhs.order != rhs.order)
            {
                return lhs.order < rhs.order;
            }
            if (lhs.type != rhs.type)
            {
                return static_cast<int>(lhs.type) < static_cast<int>(rhs.type);
            }
            return lhs.key < rhs.key;
        });

    for (const auto& e : entries)
    {
        if (e.type == UIItemType::Page)
        {
            QWidget* w = UIFactory::Instance().CreatePage(e.key, this);
            AddWidget(w, static_cast<Menu_Type>(e.menu), e.actionText);
        }
        else
        {
            QDialog* d = UIFactory::Instance().CreateDialog(e.key, this);
            AddDialog(d, static_cast<Menu_Type>(e.menu), e.actionText);
        }
    }
}

void WidgetManager::AddWidget(QWidget *widget, Menu_Type menuType, const QString &name)
{
    if (widget == NULL)
    {
        return;
    }
    QMenu *menu = m_menuList.value(menuType, nullptr);
    QAction *action = AddItemToMenu(menuType, menu, name);
    if (action == nullptr)
    {
        delete widget;
        return;
    }
    m_widgetContainer.append(widget);
    m_wgIdxMap.insert(action, m_widgetContainer.count() - 1);
    action->setProperty("item_type", "wg");
    connect(action, &QAction::triggered, this, &WidgetManager::HandleActionTriggeredSlot);
}

void WidgetManager::AddDialog(QDialog *dlg, Menu_Type type, const QString &name)
{
    if (dlg == nullptr)
    {
        return;
    }

    QMenu *menu = m_menuList.value(type, nullptr);
    QAction *action = AddItemToMenu(type, menu, name);
    if (action == nullptr)
    {
        delete dlg;
        return;
    }
    m_dialogContainer.append(dlg);
    m_dlgIdxMap.insert(action, m_dialogContainer.count() - 1);
    action->setProperty("item_type", "dlg");
    connect(action, &QAction::triggered, this, &WidgetManager::HandleActionTriggeredSlot);
}

QMenu *WidgetManager::CreateMenuItem(Menu_Type type)
{
    QMenu *menu = new QMenu(this);
    QString str = "";

    switch (type)
    {
    case TOOLS_TYPE:
        str = TR("工具");
        break;
    case OTHER_TYPE:
        str = TR("其他");
        break;
    case HELP_TYPE:
        str = TR("帮助");
        break;
    case TEST_TYPE:
        str = TR("测试");
        break;
    default:
        break;
    }
    menu->setTitle(str);
    return menu;
}

/**
 * @brief WidgetManager::AddItemToMenu 向菜单中添加选项
 * @param type 菜单类型
 * @param menu 待添加的menu对象
 * @param actionName 为空则是分隔符
 * @return
 */
QAction* WidgetManager::AddItemToMenu(Menu_Type type, QMenu *menu, const QString &actionName)
{
    if ((type < 0 || type >= TOTAL_NUM)
        || menu == nullptr)
    {
        return nullptr;
    }
    QAction *action = nullptr;
    if (actionName.trimmed().isEmpty())
    {
        menu->addSeparator();
    }
    else
    {
        action = menu->addAction(actionName);
    }

    return action;
}

void WidgetManager::InitTopMenu()
{
    QMenuBar *menuBar = new QMenuBar(this);
    for (Bit32 i = 0 ; i < TOTAL_NUM - 1; i++)
    {
        QMenu *menu = CreateMenuItem(Menu_Type(i));
        menuBar->addMenu(menu);
        m_menuList.append(menu);
    }
    ui->verticalLayout->setMenuBar(menuBar);
}

void WidgetManager::WidgetChangeSlot(int index)
{
    BaseWidget *wg = dynamic_cast<BaseWidget *>(m_widgetContainer.at(index));
    if (wg != NULL)
    {
        wg->MessageFlows(MsgData::REDRAW, "");
        wg->MessageFlows(MsgData::SETFOCUS, "");
    }
}

void WidgetManager::CallHelpDlgSlot()
{
    BaseWidget *wg = dynamic_cast<BaseWidget *>(GetCurrentWidget());
    if (wg != NULL)
    {
        QStringList contentList = wg->GetHelpText();
        HelpService::Instance().Clear();
        HelpService::Instance().ParseHelpContent(contentList);
        m_pDlgHelp->Refresh();
    }
    m_pDlgHelp->show();
}

void WidgetManager::HandleActionTriggeredSlot()
{
    QObject *origin = sender();
    QAction *action = static_cast<QAction*>(origin);
    QString type = action->property("item_type").toString();
    Bit32 index = 0;
    if (type == "wg")
    {
        index = m_wgIdxMap.value(action);
        ui->stackedWidget->setCurrentIndex(index);
    }
    else if (type == "dlg")
    {
        index = m_dlgIdxMap.value(action);
        m_dialogContainer.at(index)->exec();
    }
}
