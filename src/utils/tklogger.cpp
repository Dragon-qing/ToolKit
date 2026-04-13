/*!
 * @brief 日志工具类
 * @file logger.cpp
 * @author Dragon_qing
 * @date 2025/04/16
 */

#include <QFileInfo>
#include <QDateTime>
#include <QDir>
#include <QTextStream>
#include <QCoreApplication>

#include "common.h"
#include "sysapi.h"

#include "spdlog/spdlog.h"
#include "spdlog/sinks/rotating_file_sink.h"

#include "tklogger.h"

// #define LOG_FILE_BASE_NAME "syslog"
#define LOG_FILE_SUFFIX ".txt"
// #define LOG_MAX_NUM (3000) // 日志文件最大条数

using namespace EasyQtSql;

TKLogger &TKLogger::Instance()
{
    static TKLogger s_data;
    return s_data;
}

TKLogger::~TKLogger()
{
    spdlog::shutdown();
}

QString TKLogger::GetLogFileTypeStr(LogFileType type)
{
    QString s = "";
    switch (type)
    {
    case NORMAL_FILE:
        s = "normal_logger";
        break;
    case ERROR_FILE:
        s = "error_logger";
    default:
        break;
    }

    return s;
}

void TKLogger::InitLogger()
{
    auto logger = spdlog::rotating_logger_mt(
                GetLogFileTypeStr(LogFileType::NORMAL_FILE).toStdString(),
                GetLogPath(NORMAL_FILE).toStdString(),
                1024 * 1024 * 5,   // 5MB
                5                  // 最多5个文件
                );

    spdlog::set_default_logger(logger); // 设置为默认日志
    spdlog::set_level(spdlog::level::info); // 设置日志过滤级别
}

TKLogger::TKLogger()
{
    InitLogger(); // 初始化日志
}

Bit32 TKLogger::AddLog(LogDataType type, QString logStr, bool isFlush)
{
    if (type < DEBUG_LOG || type >= LOG_DATA_TYPE_NUM)
    {
        return -1;
    }

    switch (type)
    {
    case DEBUG_LOG:
        spdlog::debug(StrToQByte(logStr, UTF_8).data());
        break;
    case INFO_LOG:
        spdlog::info(StrToQByte(logStr, UTF_8).data());
        break;
    case WARNING_LOG:
        spdlog::warn(StrToQByte(logStr, UTF_8).data());
        break;
    case ERROR_LOG:
        spdlog::error(StrToQByte(logStr, UTF_8).data());
        break;
    case CRITICAL_LOG:
        spdlog::critical(StrToQByte(logStr, UTF_8).data());
        break;
    default:
        break;
    }

    if (isFlush)
    {
        // 立刻落盘
        spdlog::get(GetLogFileTypeStr(NORMAL_FILE).toStdString())->flush();
    }
    return 0;
}

QString TKLogger::GetLogPath(LogFileType type)
{
    QDir exeDir(GetExePath());
    QDir logDir(exeDir.filePath(GetSysPath(LOG_PATH)));
    QString path = "";
    switch (type)
    {
    case NORMAL_FILE:
        path = QDir::cleanPath(logDir.filePath(QString("%1%2").arg("log").arg(LOG_FILE_SUFFIX)));
        break;
    case ERROR_FILE:
        path = QDir::cleanPath(logDir.filePath(QString("%1%2").arg("error").arg(LOG_FILE_SUFFIX)));
        break;
    default:
        break;
    }

    return path;
}

QList<LogData> TKLogger::GetAllLog()
{
    QStringList logList;
    QDir dir(GetSysPath(LOG_PATH));
    QString fileName = QFileInfo(GetLogPath(NORMAL_FILE)).fileName();
    QString prefix = fileName.split('.').front();
    QString suffix = fileName.split('.').back();

    QStringList filters;
    filters << QString("*%1").arg(LOG_FILE_SUFFIX);
    QString patten = QString("^%1(\\.\\d+)?\\.%2$").arg(prefix).arg(suffix);
    QRegularExpression re(patten);

    for (const QString& file : dir.entryList(filters, QDir::Files)) {
        if (re.match(file).hasMatch())
        {
            logList << dir.filePath(file);
        }
    }

    return {}; // todo
}

void TKLogger::DeleteAllLog()
{
    spdlog::drop_all(); // 释放所有注册的logger，避免无法删除
    QDir dir(GetSysPath(LOG_PATH));

    QStringList filters;
    filters << QString("*.%1").arg(LOG_FILE_SUFFIX);
    QRegularExpression re("^log(\\.\\d+)?\\.txt$");

    for (const QString& file : dir.entryList(filters, QDir::Files)) {
        if (re.match(file).hasMatch())
        {
            dir.remove(file);
        }
    }

    InitLogger(); // 重新初始化注册
}

/**
 * @brief TKLogger::ClearFile 清空文件内容
 * @param filePath 文件路径
 * @return 执行结果
 */
bool TKLogger::ClearFile(const QString &filePath)
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

void TKLogger::ErrOutput(const QString &content)
{
    QString filePath = GetLogPath(ERROR_FILE);
    QFile file(filePath);
    if (!file.open(QIODevice::Append | QIODevice::Text))
    {
        return;
    }
    QTextStream out(&file);
    out.setCodec(UTF_8);
    QString timeStr = QDateTime::currentDateTime().toString("[yy-MM-dd hh:mm:ss]");
    out << QString("%1 %2\n").arg(timeStr).arg(content);
    out.flush();
    file.flush();
    file.close();
}