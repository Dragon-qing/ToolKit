/*
 * @Author: Dragon-qing
 * @Date: 2026-04-13
 * @LastEditors: Dragon-qing
 * @FilePath: \ToolKit\src\utils\tklogger.h
 * @Description: 日志工具类
 */
#ifndef TKLOGGER_H
#define TKLOGGER_H
#include <QMap>

#include "datadef.h"
#include "EasyQtSql.h"


enum LogDataType{
    DEBUG_LOG = 0,      // 调试日志
    INFO_LOG,           // 提示日志
    WARNING_LOG,        // 警告日志
    ERROR_LOG,          // 错误日志
    CRITICAL_LOG,       // 严重日志

    LOG_DATA_TYPE_NUM
};

// 日志文件种类
enum LogFileType{
    NORMAL_FILE = 0,    // 普通日志
    ERROR_FILE,         // 错误日志（该日志不走spdlog）

    LOG_FILE_TYPE_NUM
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

class TKLogger
{
public:
    static TKLogger& Instance();   // 单例
    ~TKLogger();
    
    /**
     * @brief 记录错误日志
     * @note 以文本形式记录到logs/error.txt中(使用qt操作文件，不走spdlog)
     * @param content 内容
     */
    static void ErrOutput(const QString &content);

    /**
     * @brief 添加日志
     * @param type 日志种类
     * @param logStr 日志内容
     * @param isFlush 是否立即写入
     * @return -1: 失败，0: 成功
     */
    Bit32 AddLog(LogDataType type, QString logStr, bool isFlush = true); // 添加日志

    /**
     * @brief 获取所有日志文件名
     * @return
     */
    QList<LogData> GetAllLog();

    /**
     * @brief 删除所有normal日志文件
     */
    void DeleteAllLog();
    
    /**
     * @brief: 获取日志文件种类
     * @param {LogFileType} type: 日志文件种类
     * @return {QString} 日志种类字符串
     */
    static QString GetLogFileTypeStr(LogFileType type);
    
    /**
     * @brief: 获取日志文件路径
     * @param {LogFileType} type: 日志文件种类
     * @return {*}
     */
    static QString GetLogPath(LogFileType type);
private:

    explicit TKLogger();
    bool ClearFile(const QString &filePath);

    /**
     * @brief 初始化日志
     */
    void InitLogger();
};

#endif // TKLOGGER_H
