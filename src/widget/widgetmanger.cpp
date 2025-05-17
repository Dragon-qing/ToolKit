/*!
 * @brief 界面管理类
 * @file widgetmanger.cpp
 * @date 2024/08/11
 * @author Dragonqing
 */
#include "qhotkey.h"

#include "common.h"
#include "wggeneratebugfolder.h"
#include "wgtyproaimgcleaner.h"
#include "wgbtfmake.h"
#include "wgxmleditor.h"
#include "wglog.h"
#include "wgcomrpt.h"

#include "widgetmanger.h"
#include "ui_widgetmanger.h"

#define GENERATE_WIDGET(classname, parent) new classname(parent)

WidgetManger::WidgetManger(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::WidgetManger)
{
    ui->setupUi(this);
    m_widgetContainer.clear();
    m_widgetNameList.clear();
    m_pDlgHelp = new DlgHelp(this);

    InitWidgetContainer();

    for (Bit32 i = 0; i < m_widgetContainer.count(); i++)
    {
        QWidget *tmpWidget = m_widgetContainer.at(i);
        ui->tabWidget->addTab(tmpWidget, m_widgetNameList.at(i));
    }

    connect(ui->tabWidget, &QTabWidget::currentChanged, this, &WidgetManger::WidgetChangeHandle);
    // 注册帮助页面全局快捷键
    QHotkey *helpHotKey = new QHotkey(QKeySequence("Ctrl+H"), true, qApp);
    connect(helpHotKey, &QHotkey::activated, this, &WidgetManger::CallHelpDlg);
}

WidgetManger::~WidgetManger()
{
    delete ui;
}

QWidget *WidgetManger::GetCurrentWidget()
{
    Bit32 idx = ui->tabWidget->currentIndex();

    if (idx < 0 || idx >= m_widgetContainer.count())
    {
        return NULL;
    }

    return m_widgetContainer.at(idx);
}

void WidgetManger::InitWidgetContainer()
{
    AddWidget(GENERATE_WIDGET(WgBTFMake, this), TR("BTF包制作"));
    AddWidget(GENERATE_WIDGET(WgXmlEditor, this), TR("XML编辑器"));
    // AddWidget(new WgGenerateBugFolder(this), TR("代码文件查找"));
    // AddWidget(new WgTyproaImgCleaner(this), TR("Typroa图片清理"));
    AddWidget(GENERATE_WIDGET(WgLog, this), TR("日志"));
    AddWidget(GENERATE_WIDGET(WgComRpt, this), TR("调机数据分析"));
}

void WidgetManger::AddWidget(QWidget *widget, const QString &name)
{
    if (widget == NULL)
    {
        return;
    }

    m_widgetContainer.append(widget);
    m_widgetNameList.append(name);
}

void WidgetManger::WidgetChangeHandle(int index)
{
    BaseWidget *wg = dynamic_cast<BaseWidget *>(m_widgetContainer.at(index));
    if (wg != NULL)
    {
        wg->MassageQueue(MsgData::REDRAW, "");
    }
}

void WidgetManger::CallHelpDlg()
{
    BaseWidget *wg = dynamic_cast<BaseWidget *>(GetCurrentWidget());
    if (wg != NULL)
    {
        QStringList contentList = wg->GetHelpText();
        m_pDlgHelp->SetContent(contentList);
    }
    m_pDlgHelp->show();
}
