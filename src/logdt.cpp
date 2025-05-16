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
#include <QCoreApplication>

#include "common.h"
#include "sysapi.h"

#include "logdt.h"

#define LOG_FILE_BASE_NAME "syslog"
#define LOG_FILE_SUFFIX ".db3"
#define LOG_MAX_NUM (3000) // 日志文件最大条数

using namespace EasyQtSql;

LogDt &LogDt::Instance()
{
    static LogDt s_data;
    return s_data;
}

LogDt::~LogDt()
{
    m_sqlDB.close();
}

LogDt::LogDt()
{
    m_sqlDB = QSqlDatabase::addDatabase("QSQLITE");
    m_sqlDB.setDatabaseName(GetLogPath());
    try
    {
        Transaction t(m_sqlDB); // 自动回滚非显示提交
        t.execNonQuery("CREATE TABLE IF NOT EXISTS logtb(time_stamp text, type int, content text);");
        t.commit();
    }
    catch (const DBException &e)
    {
        ErrOutput(QString("%1,%2").arg(e.lastError.text()).arg(e.lastQuery));
    }
}

void LogDt::WriteLog(Bit32 type, QString logStr)
{
    QString timeSt = QDateTime::currentDateTime().toString("yyyy-MM-dd HH:mm:ss");
    try
    {
        Transaction t(m_sqlDB);
        t.insertInto("logtb (time_stamp, type, content)")
            .values(timeSt, type, logStr).exec();
        t.commit();
    }
    catch (const DBException &e)
    {
        ErrOutput(QString("%1,%2").arg(e.lastError.text()).arg(e.lastQuery));
    }
}

Bit32 LogDt::AddLog(LogDataType type, QString logStr)
{
    if (type < DEBUG_LOG || type >= TYPE_NUM)
    {
        return -1;
    }
    if (GetLogNum() >= LOG_MAX_NUM) // 检查log是否超限
    {
        DeleteLog(0, 1000);
    }
    WriteLog(type, logStr);
    return 0;
}

QString LogDt::GetLogPath()
{
    QDir exeDir(QCoreApplication::applicationDirPath());
    QDir logDir(exeDir.filePath(GetSysPath(LOG_PATH)));
    QString path = QDir::cleanPath(logDir.filePath(QString("%1%2").arg(LOG_FILE_BASE_NAME).arg(LOG_FILE_SUFFIX)));

    return path;
}

Bit32 LogDt::GetLogNum()
{
    Bit32 num = 0;
    try
    {
        Transaction t(m_sqlDB);
        QueryResult res = t.execQuery("SELECT COUNT(*) FROM logtb;");
        res.first();
        res.fetchVars(num);
    }
    catch (const DBException &e)
    {
        ErrOutput(e.lastError.text() + e.lastQuery);
    }
    return num;
}

/**
 * @brief LogDt::GetAllLog 获取所有日志
 * @return
 */
QList<LogData> LogDt::GetAllLog()
{
    QList<LogData> logList;
    logList.clear();
    try
    {
        Transaction t(m_sqlDB);
        QueryResult res = t.execQuery("SELECT * FROM logtb;");
        while(res.next())
        {
            LogData data;
            res.fetchVars(data.m_sDate, data.m_nType, data.m_sContent);
            logList << data;
        }
    }
    catch (const DBException &e)
    {
        ErrOutput(e.lastError.text() + e.lastQuery);
    }
    return logList;
}

/**
 * @brief LogDt::DeleteLog 删除日志
 * @param from 起始位置
 * @param count 数量
 */
void LogDt::DeleteLog(Bit32 from, Bit32 count)
{
    if (from > GetLogNum() || from < 0)
    {
        return;
    }

    QString sql = QString("delete FROM logtb "\
                          "WHERE time_stamp IN (SELECT time_stamp FROM logtb ORDER BY time_stamp ASC LIMIT %1 OFFSET %2);")
                        .arg(count).arg(from);
    try
    {
        Transaction t(m_sqlDB);
        t.execNonQuery(sql);
        t.commit();
    }
    catch (const DBException &e)
    {
        ErrOutput(e.lastError.text() + e.lastQuery);
    }
}

void LogDt::DeleteAllLog()
{
    QString sql = QString("delete FROM logtb;");
    try
    {
        Transaction t(m_sqlDB);
        t.execNonQuery(sql);
        t.commit();
    }
    catch (const DBException &e)
    {
        ErrOutput(e.lastError.text() + e.lastQuery);
    }
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

