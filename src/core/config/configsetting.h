/*
 * @Author: Dragon-qing
 * @Date: 2026-04-18
 * @LastEditors: Dragon-qing
 * @FilePath: \ToolKit\src\core\configsetting\configsetting.h
 * @Description: 全局配置类，使用单例模式，封装了QSettings的相关操作
 */
#ifndef CONFIGSETTING_H
#define CONFIGSETTING_H
#include <QSettings>

class ConfigSetting
{
public:
    static ConfigSetting &Instance();
    ~ConfigSetting();

    void SetConfig(QString title, QString key, QString value);
    QString GetConfig(QString title, QString key);
    void RemoveConfig(QString title, QString key);
private:
    QSettings *m_pSetting;
    ConfigSetting();
};

#endif // CONFIGSETTING_H
