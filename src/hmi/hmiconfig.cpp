#include <QCoreApplication>

#include "hmiconfig.h"

HMIConfig &HMIConfig::Instance()
{
    static HMIConfig s_data;
    return s_data;
}

HMIConfig::~HMIConfig()
{

}

void HMIConfig::SetConfig(QString title, QString key, QString value)
{
    if (m_pSetting == NULL)
    {
        return;
    }

    m_pSetting->setValue(QString("%1/%2").arg(title, key), value);
}

QString HMIConfig::GetConfig(QString title, QString key)
{
    if (m_pSetting == NULL)
    {
        return "";
    }

    return m_pSetting->value(QString("%1/%2").arg(title, key)).toString();
}

void HMIConfig::RemoveConfig(QString title, QString key)
{
    if (m_pSetting == NULL)
    {
        return;
    }

    m_pSetting->remove(QString("%1/%2").arg(title, key));
}

HMIConfig::HMIConfig()
{
    m_pSetting = new QSettings(QCoreApplication::applicationDirPath() + "/config.ini", QSettings::IniFormat);
}
