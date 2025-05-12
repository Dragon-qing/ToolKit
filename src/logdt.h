#ifndef LOGDT_H
#define LOGDT_H
#include <QMap>

#include "datadef.h"


enum LogDataType{
    DEBUG_LOG = 0,      // 调试日志
    INFO_LOG,           // 提示日志
    WARNING_LOG,        // 警告日志
    CRITICAL_LOG,       // 严重日志
    FATAL_LOG,          // 致命日志

    TYPE_NUM
};

class LogDt
{
public:
    static LogDt& Instance();   // 单例
    ~LogDt();

    Bit32 AddLog(LogDataType type, QString logStr);
    QStringList GetLogFiles();
private:
    explicit LogDt();
    void WriteLog(QString type, QString logStr);
    QString GetAvailableFilePath();
    bool ClearFile(const QString &filePath);
};

#endif // LOGDT_H
