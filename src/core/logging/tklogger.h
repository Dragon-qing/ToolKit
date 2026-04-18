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


enum LogDataType{
    INVALID_LOG = -1,   // 无效日志类型
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
    MULTI_FILE,         // 多文件日志

    LOG_FILE_TYPE_NUM
};

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
     * @param immediate 是否立即写入
     * @return -1: 失败，0: 成功
     */
    Bit32 AddLog(LogDataType type, QString logStr, bool immediate = false);

    /**
     * @brief 获取所有日志文件名
     * @return {QStringList} 所有normal日志路径
     */
    QStringList GetAllLog(LogFileType type = NORMAL_FILE);

    /**
     * @brief 删除所有normal日志文件
     */
    void DeleteAllLog(LogFileType type);
    
    /**
     * @brief: 获取日志文件种类
     * @param {LogFileType} type: 日志文件种类
     * @return {QString} 日志种类字符串
     */
    static QString GetLogFileTypeStr(LogFileType type);
    
    /**
     * @brief: 获取日志文件路径
     * @param {LogFileType} type: 日志文件种类
     * @return {QString} 日志文件绝对路径
     */
    static QString GetLogPath(LogFileType type);

    /**
     * @brief: 从字符串获取日志类型
     * @param {QString} &typeStr: spdlog日志类型字符串
     * @return {LogDataType} 日志类型
     */   
    static LogDataType Str2LogDataType(const QString &typeStr);

    /**
     * @brief: 日志类型转换为字符串
     * @param {LogDataType} type: 日志枚举类型
     * @return {QString} 日志类型字符串
     */
    static QString LogDataType2Str(LogDataType type);

    /**
     * @brief: 根据日志文件种类获取日志文件名
     * @param {LogFileType} type: 日志文件种类
     * @return {QString} 日志文件名,不包含后缀
     */
    static QString Type2LogFileName(LogFileType type);
private:
    explicit TKLogger();

    /**
     * @brief 初始化日志
     */
    void InitLogger();
};

#endif // TKLOGGER_H
