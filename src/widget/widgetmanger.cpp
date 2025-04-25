#include "common.h"
#include "wggeneratebugfolder.h"
#include "wgtyproaimgcleaner.h"
#include "wgbtfmake.h"
#include "wgxmleditor.h"

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
