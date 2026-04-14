#ifndef DATALOG_H
#define DATALOG_H

#include <QStandardItemModel>

#include "common.h"

struct LogData // 日志数据结构
{
    QString m_sDate;    // 时间
    QString m_sType;      // 类型
    QString m_sContent; // 内容
    LogData() {
        m_sDate = "";
        m_sType = "";
        m_sContent = "";
    }
};

class DataLog
{
public:
    static DataLog &GetInstance();
    DataLog();

    /**
     * @brief:  刷新日志文件列表
     * @return {void}
     */
    void RefreshLogFiles();

    /**
     * @brief: 读取数据并加载到model中
     * @param {QStandardItemModel} *model: 
     * @return {void}
     */
    void LoadLog(QStandardItemModel *model);
private:
    QStringList m_slFiles; // 日志文件列表
    QString m_sCurFile; // 当前日志文件
};

#endif // DATALOG_H
