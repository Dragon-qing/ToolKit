/*!
 * @brief 界面管理类
 * @file widgetmanger.cpp
 * @date 2024/08/11
 * @author Dragonqing
 */
#include "qhotkey.h"

#include "common.h"
#include "wgtyproaimgcleaner.h"
#include "wgarchivemaker.h"
#include "wgxmleditor.h"
#include "wglog.h"
#include "wgcomrpt.h"
#include "wgrenametool.h"

#include "dlgabout.h"

#include "widgetmanger.h"
#include "ui_widgetmanger.h"

#define GENERATE_ITEM(classname, parent) new classname(parent)

WidgetManger::WidgetManger(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::WidgetManger)
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

    connect(ui->stackedWidget, &QStackedWidget::currentChanged, this, &WidgetManger::WidgetChangeSlot);
    // 注册帮助页面全局快捷键
    QHotkey *helpHotKey = new QHotkey(QKeySequence("Ctrl+H"), true, qApp);
    connect(helpHotKey, &QHotkey::activated, this, &WidgetManger::CallHelpDlgSlot);
}

WidgetManger::~WidgetManger()
{
    delete ui;
}

QWidget *WidgetManger::GetCurrentWidget()
{
    Bit32 idx = ui->stackedWidget->currentIndex();

    if (idx < 0 || idx >= m_widgetContainer.count())
    {
        return NULL;
    }

    return m_widgetContainer.at(idx);
}

void WidgetManger::InitWidgetContainer()
{
    Menu_Type type = TOOLS_TYPE;
    // 工具菜单组
    AddWidget(GENERATE_ITEM(WgArchiveMaker, this), type, TR("压缩包制作"));
    AddWidget(GENERATE_ITEM(WgXmlEditor, this), type, TR("XML编辑器"));
    // AddWidget(new WgTyproaImgCleaner(this), TR("Typroa图片清理"));
    AddWidget(GENERATE_ITEM(WgComRpt, this), type, TR("调机数据分析"));
    AddWidget(GENERATE_ITEM(WgRenameTool, this), type, TR("文件重命名"));
    type = HELP_TYPE;
    // 帮助菜单组
    AddWidget(GENERATE_ITEM(WgLog, this), type, TR("日志"));
    AddDialog(GENERATE_ITEM(DlgAbout, this), type, TR("关于"));
}

void WidgetManger::AddWidget(QWidget *widget, Menu_Type menuType, const QString &name)
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
    connect(action, &QAction::triggered, this, &WidgetManger::HandleActionTriggeredSlot);
}

void WidgetManger::AddDialog(QDialog *dlg, Menu_Type type, const QString &name)
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
    connect(action, &QAction::triggered, this, &WidgetManger::HandleActionTriggeredSlot);
}

QMenu *WidgetManger::CreateMenuItem(Menu_Type type)
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
 * @brief WidgetManger::AddItemToMenu 向菜单中添加选项
 * @param type 菜单类型
 * @param menu 待添加的menu对象
 * @param actionName 为空则是分隔符
 * @return
 */
QAction* WidgetManger::AddItemToMenu(Menu_Type type, QMenu *menu, const QString &actionName)
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

void WidgetManger::InitTopMenu()
{
    QMenuBar *menuBar = new QMenuBar(this);
    for (Bit32 i = 0 ; i < TOTAL_NUM; i++)
    {
        QMenu *menu = CreateMenuItem(Menu_Type(i));
        menuBar->addMenu(menu);
        m_menuList.append(menu);
    }
    ui->verticalLayout->setMenuBar(menuBar);
}

void WidgetManger::WidgetChangeSlot(int index)
{
    BaseWidget *wg = dynamic_cast<BaseWidget *>(m_widgetContainer.at(index));
    if (wg != NULL)
    {
        wg->MessageFlows(MsgData::REDRAW, "");
        wg->MessageFlows(MsgData::SETFOCUS, "");
    }
}

void WidgetManger::CallHelpDlgSlot()
{
    BaseWidget *wg = dynamic_cast<BaseWidget *>(GetCurrentWidget());
    if (wg != NULL)
    {
        QStringList contentList = wg->GetHelpText();
        m_pDlgHelp->SetContent(contentList);
    }
    m_pDlgHelp->show();
}

void WidgetManger::HandleActionTriggeredSlot()
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
