#ifndef DATACONFIG_H
#define DATACONFIG_H
#include <QSettings>

class DataConfig
{
public:
    static DataConfig &Instance();
    ~DataConfig();

    void SetConfig(QString title, QString key, QString value);
    QString GetConfig(QString title, QString key);
    void RemoveConfig(QString title, QString key);
private:
    QSettings *m_pSetting;
    DataConfig();
};

#endif // DATACONFIG_H
