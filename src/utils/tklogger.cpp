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
#include <QRegularExpression>

#include "common.h"
#include "sysapi.h"

#include "spdlog/spdlog.h"
#include "spdlog/sinks/rotating_file_sink.h"
#include "spdlog/sinks/stdout_color_sinks.h"

#include "tklogger.h"

#define LOG_FILE_SUFFIX ".txt"

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
        break;
    case MULTI_FILE:
        s = "multi_logger";
        break;
    default:
        break;
    }

    return s;
}

void TKLogger::InitLogger()
{
    auto file_sink = std::make_shared<spdlog::sinks::rotating_file_sink_mt>(
                GetLogPath(NORMAL_FILE).toStdString(),
                1024 * 1024 * 10,   // 10MB
                5                  // 最多5个文件
    );
    auto console_sink = std::make_shared<spdlog::sinks::stdout_color_sink_mt>();

    console_sink->set_pattern("[%H:%M:%S.%e] [%^%l%$] %v");
    console_sink->set_level(spdlog::level::info);
    file_sink->set_pattern("[%Y-%m-%d %H:%M:%S.%e] [%l] %v");
    file_sink->set_level(spdlog::level::debug);

    // 创建 logger
    auto logger = std::make_shared<spdlog::logger>(
        GetLogFileTypeStr(MULTI_FILE).toStdString(), 
        spdlog::sinks_init_list{console_sink, file_sink}
    );
    // 注册
    spdlog::register_logger(logger);
    // 设置为默认日志
    spdlog::set_default_logger(logger); 
    // 设置日志过滤级别
    spdlog::set_level(spdlog::level::info); 
    // 每3秒自动刷新日志
    spdlog::flush_every(std::chrono::seconds(3)); 
}

QString TKLogger::Type2LogFileName(LogFileType type)
{
    if (type < 0 || type >= LOG_FILE_TYPE_NUM)
    {
        return QString();
    }

    switch (type)
    {
    case NORMAL_FILE:
        return QString("log");
        break;
    case ERROR_FILE:
        return QString("error");
        break;
    default:
        break;
    }
    return QString();
}

TKLogger::TKLogger()
{
    InitLogger(); // 初始化日志
}

Bit32 TKLogger::AddLog(LogDataType type, QString logStr, bool immediate)
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

    if (immediate)
    {
        // 立刻落盘
        spdlog::get(GetLogFileTypeStr(MULTI_FILE).toStdString())->flush();
    }
    return 0;
}

QString TKLogger::GetLogPath(LogFileType type)
{
    if (type < 0 || type >= LOG_FILE_TYPE_NUM)
    {
        return QString();
    }

    QDir logDir(GetSysPath(LOG_PATH));
    QString path = "";
    path = QDir::cleanPath(logDir.filePath(QString("%1%2").arg(Type2LogFileName(type)).arg(LOG_FILE_SUFFIX)));

    return path;
}

QStringList TKLogger::GetAllLog(LogFileType type)
{
    QStringList logList;
    QDir dir(GetSysPath(LOG_PATH));
    QString fileName = QFileInfo(GetLogPath(type)).fileName();
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

    return logList;
}

void TKLogger::DeleteAllLog(LogFileType type)
{
    if (type < 0 || type >= LOG_FILE_TYPE_NUM)
    {
        AddLog(ERROR_LOG, QString("无效的日志文件类型: %1").arg(type));
        return;
    }
    spdlog::shutdown(); // 释放所有的logger,并flush,避免无法删除
    QDir dir(GetSysPath(LOG_PATH));

    QStringList filters;
    filters << QString("*%1").arg(LOG_FILE_SUFFIX);
    QString patten = QString("^%1(\\.\\d+)?\\%2$").arg(Type2LogFileName(type)).arg(LOG_FILE_SUFFIX);
    QRegularExpression re(patten);

    for (const QString& file : dir.entryList(filters, QDir::Files)) {
        if (re.match(file).hasMatch())
        {
            dir.remove(file);
        }
    }

    InitLogger(); // 重新初始化注册
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

LogDataType TKLogger::Str2LogDataType(const QString &typeStr)
{
    if (typeStr == "debug")
    {
        return DEBUG_LOG;
    }
    else if (typeStr == "info")
    {
        return INFO_LOG;
    }
    else if (typeStr == "warning")
    {
        return WARNING_LOG;
    }
    else if (typeStr == "error")
    {
        return ERROR_LOG;
    }
    else if (typeStr == "critical")
    {
        return CRITICAL_LOG;
    }

    return INVALID_LOG;
}

QString TKLogger::LogDataType2Str(LogDataType type)
{
    QString typeStr = "";
    switch (type)
    {
    case DEBUG_LOG:
        typeStr = "debug";
        break;
    case INFO_LOG:
        typeStr = "info";
        break;
    case WARNING_LOG:
        typeStr = "warning";
        break;
    case CRITICAL_LOG:
        typeStr = "critical";
        break;
    case ERROR_LOG:
        typeStr = "error";
        break;
    default:
        break;
    }

    return typeStr;
}
