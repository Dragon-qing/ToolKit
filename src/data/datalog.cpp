/*!
 * @brief 日志数据层
 * @file datalog.cpp
 * @author Dragon_qing
 * @date 2025/04/27
 */
#include <QFile>
#include <QTextStream>
#include <QRegularExpression>

#include "tklogger.h"
#include "EasyQtSql.h"

#include "datalog.h"

DataLog &DataLog::GetInstance()
{
    static DataLog sdata;
    return sdata;
}

DataLog::DataLog()
{
    m_slFiles.clear();
    m_sCurFile = TKLogger::GetLogPath(NORMAL_FILE);
}

void DataLog::RefreshLogFiles()
{
    m_slFiles.clear();
    m_slFiles = TKLogger::Instance().GetAllLog();
}

void DataLog::LoadLog(QStandardItemModel *model)
{
    if (model == nullptr)
    {
        return;
    }
    
    QFile file(m_sCurFile);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        TKLogger::Instance().AddLog(ERROR_LOG, QObject::TR("无法打开日志文件: %1").arg(m_sCurFile));
        return;
    }
    QTextStream in(&file);
    while (!in.atEnd())
    {
        QString line = in.readLine();
        // 使用正则表达式解析日志行
        QRegularExpression re(R"(\[(.*?)\]\s\[(.*?)\]\s(.*))");
        QRegularExpressionMatch match = re.match(line);
        if (match.hasMatch())
        {
            LogData data;
            data.m_sDate = match.captured(1); // 时间
            data.m_sType = match.captured(2); // 类型
            data.m_sContent = match.captured(3); // 内容

            QStandardItem *typeItem = new QStandardItem(data.m_sType );
            typeItem->setTextAlignment(Qt::AlignCenter); // 设置类型列文本居中
            model->appendRow({
                new QStandardItem(data.m_sDate),
                typeItem,
                new QStandardItem(data.m_sContent)
            });
        }
    }
    model->sort(0, Qt::DescendingOrder); // 按时间降序排序
    file.close();
}
