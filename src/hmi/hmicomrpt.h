#ifndef HMICOMRPT_H
#define HMICOMRPT_H

#include <QString>
#include <QMap>
#include <QVariant>
#include <QThread>
#include <QObject>

#include "common.h"
QT_BEGIN_NAMESPACE
class ReadFileThread;
QT_END_NAMESPACE

class HmiComRpt : public QObject
{
    Q_OBJECT
public:
    HmiComRpt(QObject *parent = nullptr);
    ~HmiComRpt();
    static HmiComRpt &Instance();
    void SetPath(QString path);
private:
    QString m_sPath; // 数据文件路径
    QMap<QString, QVariant> m_infoMap; // 文件头数据
    QMap<QString, QVariant> m_configMap; // 配置数据
    QList<QVector<Bit64>> m_dataList; // 数据部分
    ReadFileThread *m_pThread;

    void Clear();


};

class ReadFileThread : public QThread
{
    Q_OBJECT
public:
    ReadFileThread(QObject *parent = nullptr);
    void SetConfig(QString path, QMap<QString, QVariant>* info, QMap<QString, QVariant>* config, QList<QVector<Bit64>>* data);
protected:
    void run() override;

private:
    enum _Context_Type{
        INVALID = 0,    // 无效
        INFO_PART = 1,  // 文件头部分
        CONFIG_PART,    // 配置部分
        DATA_PART       // 数据部分
    };

    QString m_sPath; // 数据文件路径
    QMap<QString, QVariant>* m_pInfoMap; // 文件头数据
    QMap<QString, QVariant>* m_pConfigMap; // 配置数据
    QList<QVector<Bit64>>* m_pDataList; // 数据部分

    Bit32 ProcessFile(); // 处理解析数据文件
    void ParseLine(Bit32 type, QString tmp);
};

#endif // HMICOMRPT_H
