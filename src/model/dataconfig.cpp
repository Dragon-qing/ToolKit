#include <QCoreApplication>

#include "dataconfig.h"

DataConfig &DataConfig::Instance()
{
    static DataConfig s_data;
    return s_data;
}

DataConfig::~DataConfig()
{

}

void DataConfig::SetConfig(QString title, QString key, QString value)
{
    if (m_pSetting == NULL)
    {
        return;
    }

    m_pSetting->setValue(QString("%1/%2").arg(title, key), value);
}

QString DataConfig::GetConfig(QString title, QString key)
{
    if (m_pSetting == NULL)
    {
        return "";
    }

    return m_pSetting->value(QString("%1/%2").arg(title, key)).toString();
}

void DataConfig::RemoveConfig(QString title, QString key)
{
    if (m_pSetting == NULL)
    {
        return;
    }

    m_pSetting->remove(QString("%1/%2").arg(title, key));
}

DataConfig::DataConfig()
{
    m_pSetting = new QSettings(QCoreApplication::applicationDirPath() + "/config.ini", QSettings::IniFormat);
}
