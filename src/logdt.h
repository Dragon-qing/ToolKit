#ifndef LOGDT_H
#define LOGDT_H
#include <QMap>

#include "datadef.h"
#include "EasyQtSql.h"


enum LogDataType{
    DEBUG_LOG = 0,      // 调试日志
    INFO_LOG,           // 提示日志
    WARNING_LOG,        // 警告日志
    CRITICAL_LOG,       // 严重日志
    FATAL_LOG,          // 致命日志

    TYPE_NUM
};

typedef struct LogData
{
    QString m_sDate;    // 时间
    Bit32 m_nType;      // 类型
    QString m_sContent; // 内容
    LogData() {
        m_sDate = "";
        m_nType = -1;
        m_sContent = "";
    }
}LogData;

class LogDt
{
public:
    static LogDt& Instance();   // 单例
    ~LogDt();

    Bit32 AddLog(LogDataType type, QString logStr); // 添加日志
    QString GetLogPath();
    Bit32 GetLogNum(); // 获取日志数量
    QList<LogData> GetAllLog();
    void DeleteLog(Bit32 from, Bit32 count);
private:
    QSqlDatabase m_sqlDB;

    explicit LogDt();
    void WriteLog(Bit32 type, QString logStr);
    bool ClearFile(const QString &filePath);
};

#endif // LOGDT_H
