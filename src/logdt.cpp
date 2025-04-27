/*!
 * @brief 日志模块
 * @file logdt.cpp
 * @author Dragon_qing
 * @date 2025/04/16
 */

#include <QFileInfo>
#include <QDateTime>
#include <QDir>
#include <QTextStream>

#include "common.h"
#include "syspathmanager.h"

#include "logdt.h"

#define LOG_FILE_BASE_NAME "syslog"
#define FILE_BAK_MAX_NUM (10) // 日志文件最大备份个数
#define FILE_MAX_SIZE (50 * 1024 * 1024) // 单个文件最大为50MB

LogDt &LogDt::Instance()
{
    static LogDt s_data;
    return s_data;
}

LogDt::~LogDt()
{

}

LogDt::LogDt()
{
}

void LogDt::WriteLog(QString type, QString logStr)
{
    if (type.isEmpty() || logStr.isEmpty())
    {
        return;
    }

    QString filePath = GetAvailableFilePath();

    QFile file(filePath);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text | QIODevice::Append))
    {
        return;
    }

    QTextStream textStream(&file);
    textStream.setCodec("UTF-8");
    textStream.reset();
    QString currentTime = QDateTime::currentDateTime().toString("[yyyy-MM-dd HH:mm:ss]");
    textStream << QString("%1(%2):%3\n").arg(currentTime).arg(type).arg(logStr);
    file.flush();
    file.close();
}

QString LogDt::GetAvailableFilePath()
{
    QDir logDir(GetSysPath(LOG_PATH));
    QString path = "";
    QDateTime modifyTime = QDateTime::currentDateTime();
    Bit32 oldestIdx = 0; // 最旧的log文件序号；
    if (!logDir.exists())
    {
        logDir.mkpath(logDir.absolutePath());
    }

    for (Bit32 i = 0; i < FILE_BAK_MAX_NUM; i++)
    {
        QString filename = QString("%1_%2.txt").arg(LOG_FILE_BASE_NAME).arg(i, 2, 10, QChar('0'));
        QString tmpPath = QDir::toNativeSeparators(logDir.filePath(filename));
        QFileInfo info(tmpPath);
        if (!info.exists()) // 不存在
        {
            path = tmpPath;
            break;
        }
        else
        {
            if (info.size() < FILE_MAX_SIZE)
            {
                path = tmpPath;
                break;
            }
            // 超过单文件最大大小
            if(info.lastModified() < modifyTime)
            {
                modifyTime = info.lastModified();
                oldestIdx = i;
            }

        }
    }
    if (path.isEmpty())
    {
        path = QString("%1_%2.txt").arg(LOG_FILE_BASE_NAME).arg(oldestIdx, 2, 10, QChar('0')); // 取最旧的文件名
        ClearFile(path);
    }

    return path;
}

Bit32 LogDt::AddLog(LogDataType type, QString logStr)
{
    if (type < DEBUG_LOG || type >= TYPE_NUM)
    {
        return -1;
    }
    QString typeStr = "";
    switch (type)
    {
    case DEBUG_LOG:
        typeStr = "D";
        break;
    case INFO_LOG:
        typeStr = "I";
        break;
    case WARNING_LOG:
        typeStr = "W";
        break;
    case CRITICAL_LOG:
        typeStr = "C";
        break;
    case FATAL_LOG:
        typeStr = "F";
        break;
    default:
        break;
    }
    WriteLog(typeStr, logStr);
    return 0;
}

/**
 * @brief LogDt::GetLogFiles 获取所有的log文件路径
 * @return
 */
QStringList LogDt::GetLogFiles()
{
    QDir logDir(GetSysPath(LOG_PATH));
    QStringList fileList;
    if (!logDir.exists())   // log目录不存在
    {
        return fileList;
    }

    QStringList files = logDir.entryList();
    foreach(const QString &str, files)
    {
        if (str.startsWith(LOG_FILE_BASE_NAME))
        {
            fileList.append(logDir.absoluteFilePath(str));
        }
    }

    return fileList;
}

/**
 * @brief LogDt::ClearFile 清空文件内容
 * @param filePath 文件路径
 * @return 执行结果
 */
bool LogDt::ClearFile(const QString &filePath)
{
    QFile file(filePath);
    if (file.open(QIODevice::WriteOnly | QIODevice::Truncate)) {
        file.flush();
        file.close();  // 文件已经被清空，立即关闭
        return true;
    } else {
        return false;
    }
}

