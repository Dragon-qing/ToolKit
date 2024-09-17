#include "common.h"
#include "wggeneratebugfolder.h"
#include "wgtyproaimgcleaner.h"
#include "wgbtfmake.h"
#include "wgtestpython.h"

#include "widgetmanger.h"
#include "ui_widgetmanger.h"

WidgetManger::WidgetManger(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::WidgetManger)
{
    ui->setupUi(this);
    m_widgetContainer.clear();
    m_widgetNameList.clear();

    InitWidgetContainer();

    for (Bit32 i = 0; i < m_widgetContainer.count(); i++)
    {
        QWidget *tmpWidget = m_widgetContainer.at(i);
        ui->tabWidget->addTab(tmpWidget, m_widgetNameList.at(i));
    }
}

WidgetManger::~WidgetManger()
{
    delete ui;
}

void WidgetManger::InitWidgetContainer()
{
    AddWidget(new WgBTFMake(this), TR("BTF包制作"));
    AddWidget(new WgGenerateBugFolder(this), TR("代码文件查找"));
    AddWidget(new WgTyproaImgCleaner(this), TR("Typroa图片清理"));
    AddWidget(new WgTestPython(this), TR("测试python"));
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
