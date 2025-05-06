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
    QTextStream in(&file);
    Bit32 type = INVALID; // 文件中的part类型
    QRegularExpression re("^\\[.*\\]$");
    in.setCodec(UTF_8);
    while(!in.atEnd())
    {
        QString tmp = in.readLine();
        ParseLine(type, tmp);
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
    file.close();
    return 0;
}

void ReadFileThread::ParseLine(Bit32 type, QString tmp)
{
    if (type == INVALID || tmp.isEmpty())
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

void ReadFileThread::run()
{
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
