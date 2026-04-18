#ifndef WIDGETMANAGER_H
#define WIDGETMANAGER_H

#include <QWidget>
#include <QMenu>

#include "basewidget.h"

#include "dlghelp.h"

namespace Ui {
class WidgetManager;
}

class WidgetManager : public QWidget
{
    Q_OBJECT

public:
    explicit WidgetManager(QWidget *parent = nullptr);
    ~WidgetManager();
    QWidget *GetCurrentWidget();

private:
    enum Menu_Type{
        TOOLS_TYPE = 0,     // 工具
        OTHER_TYPE,         // 其他
        HELP_TYPE,          // 帮助


        TEST_TYPE,          // 测试
        TOTAL_NUM
    };
    Ui::WidgetManager *ui;

    QList<QWidget *> m_widgetContainer; // 窗口容器
    QList<QDialog *> m_dialogContainer; // 对话框容器
    DlgHelp *m_pDlgHelp; // 全局通用的帮助弹窗
    QList<QMenu*> m_menuList; // 菜单组
    QMap<QAction*, Bit32> m_wgIdxMap; // action索引表(widget)
    QMap<QAction*, Bit32> m_dlgIdxMap; // action索引表(Dialog)

    void AddWidget(QWidget *widget, Menu_Type menuType, const QString &name);
    void AddDialog(QDialog *dlg, Menu_Type type, const QString &name);
    void InitWidgetContainer();
    void InitTopMenu();
    QMenu *CreateMenuItem(Menu_Type type);
    QAction* AddItemToMenu(Menu_Type type, QMenu *menu, const QString &actionName);
private slots:
    void WidgetChangeSlot(int index);
    void CallHelpDlgSlot();
    void HandleActionTriggeredSlot();

};

#endif // WIDGETMANAGER_H
