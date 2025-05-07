/*!
 * @brief 调机数据层
 * @file hmicomrpt.cpp
 * @author Dragon_qing
 * @date 2025/05/06
 */
#include <QObject>
#include <QFileInfo>
#include <QFile>
#include <QTextStream>
#include <QRegularExpression>

#include "hmicomrpt.h"
#include "logdt.h"

HmiComRpt::HmiComRpt(QObject *parent)
    : QObject(parent)
{
    m_sPath = "";
    m_infoMap.clear();
    m_configMap.clear();
    m_dataList.clear();
    m_pThread = new ReadFileThread(this);
}

HmiComRpt::~HmiComRpt()
{
    m_pThread->terminate();
}

HmiComRpt &HmiComRpt::Instance()
{
    static HmiComRpt s_data;
    return s_data;
}

void HmiComRpt::SetPath(QString path)
{
    if (path.isEmpty())
    {
        LogDt::Instance().AddLog(WARNING_LOG, QObject::TR("%1路径错误").arg(path));
    }

    QFileInfo info(path);
    if (!info.exists())
    {
        LogDt::Instance().AddLog(WARNING_LOG, QObject::TR("%1文件不存在").arg(info.fileName()));
    }

    m_sPath = path;
    m_pThread->SetConfig(path, &m_infoMap, &m_configMap, &m_dataList);
    m_pThread->start();
}

ReadFileThread *HmiComRpt::GetThreadClass()
{
    return m_pThread;
}

/**
 * @brief HmiComRpt::GetTotalPosNum 获取数据点总数(单种采样)
 * @return 数据点总数
 */
Bit32 HmiComRpt::GetTotalPosNum()
{
    if (m_dataList.isEmpty())
    {
        return 0;
    }

    return m_dataList.first().size();
}

/**
 * @brief HmiComRpt::GetValue 获取数据接口
 * @param type 数据种类
 * @param key 键值索引
 * @param subKey 子键值索引
 * @return 返回数据值
 */
QVariant HmiComRpt::GetValue(Bit32 type, QVariant key, QVariant subKey)
{
    if (type == INFO_PART)
    {
        return m_infoMap.value(key.toString());
    }
    else if (type == CONFIG_PART)
    {
        return m_configMap.value(key.toString());
    }
    else if (type == DATA_PART)
    {
        Bit32 row = key.toInt();
        if (row < 0 || row >= m_dataList.count())
        {
            return QVariant();
        }
        Bit32 index = subKey.toInt();
        Bit64 rawValue = m_dataList.value(row).value(index);
        return QVariant(rawValue);
    }

    return QVariant();
}

void HmiComRpt::Test()
{
    Bit32 total = m_dataList.at(1).size();
    QString context = "";
    for (Bit32 i = 0; i < total; i++)
    {
        context += QString::number(m_dataList.at(1).value(i));
    }
    ComDebug(context);
}

QString HmiComRpt::GetFileName()
{
    return QFileInfo(m_sPath).fileName();
}

void HmiComRpt::Clear()
{
    m_sPath = "";
    m_infoMap.clear();
    m_configMap.clear();
    m_dataList.clear();
}

Bit32 ReadFileThread::ProcessFile()
{
    QFile file(m_sPath);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        LogDt::Instance().AddLog(WARNING_LOG, QObject::TR("文件打开失败"));
        return -1;
    }
    Bit64 totalSize = QFileInfo(m_sPath).size();
    if (totalSize == 0)
    {
        LogDt::Instance().AddLog(WARNING_LOG, QObject::TR("文件为空"));
        return -1;
    }

    Bit64 currentSize = 0;
    QTextStream in(&file);
    Bit32 type = INVALID; // 文件中的part类型
    QRegularExpression re("^\\[.*\\]$");

    Bit32 count = 0;
    QStringList batch;
    in.setCodec(UTF_8);
    while(!in.atEnd())
    {

        QString tmp = "";
        if (type == DATA_PART)
        {
            batch << in.readLine();
            if (count % 1000 == 0)
            {
                ParseLines(type, batch);
                batch.clear();
            }
        }
        else
        {
            tmp = in.readLine();
            ParseLine(type, tmp);
        }
        ParseLine(type, tmp);
        currentSize = file.pos();
        int eta = static_cast<int>(currentSize * 1.0 / totalSize * 100);
        if (++count % 1000 == 0)
        {
            emit ProcessSignal(eta);
        }
        if (tmp.contains("[INFO]"))
        {
            type = INFO_PART;
        }
        else if (tmp.contains("[CONFIG]"))
        {
            type = CONFIG_PART;
        }
        else if (tmp.contains("[DATA]"))
        {
            type = DATA_PART;
        }
        else if (re.match(tmp).hasMatch())
        {
            type = INVALID;
        }
    }
    if (!batch.isEmpty())
    {
        ParseLines(type, batch);
        batch.clear();
    }
    emit ProcessSignal(100);
    file.close();
    return 0;
}

void ReadFileThread::ParseLine(Bit32 type, QString tmp)
{
    if (type == INVALID || tmp.isEmpty() || tmp.startsWith("["))
    {
        return;
    }
    switch (type)
    {
    case INFO_PART:
    {
        QStringList infoList = tmp.split("=");
        if (infoList.count() >= 2)
        {
            m_pInfoMap->insert(infoList.at(0), QVariant(infoList.at(1)));
        }
        break;
    }
    case CONFIG_PART:
    {
        QStringList configList;
        if (tmp.startsWith("CONF_VAL"))
        {
            configList = tmp.split(":").last().split(";");
            foreach (QString str, configList)
            {
                QStringList subList = str.split("=");
                if (subList.count() >= 2)
                {
                    m_pConfigMap->insert(subList.at(0), QVariant(subList.at(1)));
                }
            }
        }
        else
        {
            configList = tmp.split("=");
            if (configList.count() >= 2)
            {
                m_pConfigMap->insert(configList.at(0), QVariant(configList.at(1)));
            }
        }
        break;
    }
    case DATA_PART:
    {
        QStringList dataList = tmp.split(";");
        for (Bit32 i = 0; i < dataList.count(); i++)
        {
            QString str = dataList.at(i);
            str = str.trimmed();
            bool ok = false;
            Bit64 data = str.toLongLong(&ok);
            if (str.isEmpty() || !ok)
            {
                continue;
            }
            QVector<Bit64> dataVec = m_pDataList->value(i);
            if (i + 1 > m_pDataList->count())
            {
                dataVec.append(data);
                m_pDataList->append(dataVec);
            }
            else
            {
                dataVec.append(data);
                m_pDataList->replace(i, dataVec);
            }
        }
        break;
    }
    default:
        break;
    }
}

void ReadFileThread::ParseLines(Bit32 type, QStringList list)
{
    foreach(QString str, list)
    {
        ParseLine(type, str);
    }
}

void ReadFileThread::run()
{
    emit ProcessSignal(0);
    ProcessFile();
}

ReadFileThread::ReadFileThread(QObject *parent)
    : QThread(parent)
{

}

void ReadFileThread::SetConfig(QString path, QMap<QString, QVariant> *info, QMap<QString, QVariant> *config, QList<QVector<Bit64> > *data)
{
    m_sPath = path;
    m_pInfoMap = info;
    m_pConfigMap = config;
    m_pDataList = data;
}
