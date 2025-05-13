/*!
 * @brief 日志数据层
 * @file hmilog.cpp
 * @author Dragon_qing
 * @date 2025/04/27
 */
#include <QFile>
#include <QTextStream>
#include <QRegularExpression>

#include "logdt.h"
#include "EasyQtSql.h"

#include "hmilog.h"

HmiLog &HmiLog::GetInstance()
{
    static HmiLog sdata;
    return sdata;
}

HmiLog::HmiLog()
{
}

void HmiLog::LoadLog(QStandardItemModel *model)
{
    if (model == NULL)
    {
        return;
    }
    QList<LogData> logList = LogDt::Instance().GetAllLog();
    for (Bit32 i = logList.count() - 1, row = 0; i >= 0; i--, row++)
    {
        const LogData &data = logList.at(i);
        QString time = data.m_sDate;
        QString type = TransLogType2Str(data.m_nType);
        QString context = data.m_sContent;

        QStandardItem *timeItem = new QStandardItem(time);
        QStandardItem *typeItem = new QStandardItem(type);
        typeItem->setTextAlignment(Qt::AlignCenter);
        QStandardItem *contextItem = new QStandardItem(context);
        model->setItem(row, 0, timeItem);
        model->setItem(row, 1, typeItem);
        model->setItem(row, 2, contextItem);
    }
}

QString HmiLog::TransLogType2Str(Bit32 type)
{
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

    return typeStr;
}
