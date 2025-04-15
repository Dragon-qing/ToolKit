#ifndef HMICONFIG_H
#define HMICONFIG_H
#include <QSettings>

class HMIConfig
{
public:
    static HMIConfig &Instance();
    ~HMIConfig();

    void SetConfig(QString title, QString key, QString value);
    QString GetConfig(QString title, QString key);
    void RemoveConfig(QString title, QString key);
private:
    QSettings *m_pSetting;
    HMIConfig();
};

#endif // HMICONFIG_H
