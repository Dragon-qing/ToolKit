/*
 * @Author: Dragon-qing
 * @Date: 2026-04-15
 * @LastEditors: Dragon-qing
 * @FilePath: \ToolKit\src\core\configsetting\configsetting.cpp
 * @Description: 全局配置类，使用单例模式，封装了QSettings的相关操作
 */
#include <QCoreApplication>

#include "configsetting.h"

ConfigSetting &ConfigSetting::Instance()
{
    static ConfigSetting s_data;
    return s_data;
}

ConfigSetting::~ConfigSetting()
{

}

void ConfigSetting::SetConfig(QString title, QString key, QString value)
{
    if (m_pSetting == NULL)
    {
        return;
    }

    m_pSetting->setValue(QString("%1/%2").arg(title, key), value);
}

QString ConfigSetting::GetConfig(QString title, QString key)
{
    if (m_pSetting == NULL)
    {
        return "";
    }

    return m_pSetting->value(QString("%1/%2").arg(title, key)).toString();
}

void ConfigSetting::RemoveConfig(QString title, QString key)
{
    if (m_pSetting == NULL)
    {
        return;
    }

    m_pSetting->remove(QString("%1/%2").arg(title, key));
}

ConfigSetting::ConfigSetting()
{
    m_pSetting = new QSettings(QCoreApplication::applicationDirPath() + "/config.ini", QSettings::IniFormat);
}
