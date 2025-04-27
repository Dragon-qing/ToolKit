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

#include "hmilog.h"

HmiLog &HmiLog::GetInstance()
{
    static HmiLog sdata;
    return sdata;
}

HmiLog::HmiLog() {}

void HmiLog::LoadLog(QStandardItemModel *model)
{
    if (model == NULL)
    {
        return;
    }
    QStringList logFiles = LogDt::Instance().GetLogFiles();
    Bit32 row = 0;
    foreach (const QString &filePath, logFiles)
    {
        QFile file(filePath);
        if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
        {
            return;
        }
        QTextStream in(&file);
        in.setCodec(UTF_8);
        while (!in.atEnd())
        {
            QString rawStr = in.readLine();
            QString time = "";
            QString type = "";
            QString context = "";
            ProcessRawLog(rawStr, time, type, context);
            QStandardItem *timeItem = new QStandardItem(time);
            QStandardItem *typeItem = new QStandardItem(type);
            typeItem->setTextAlignment(Qt::AlignCenter);
            QStandardItem *contextItem = new QStandardItem(context);
            model->setItem(row, 0, timeItem);
            model->setItem(row, 1, typeItem);
            model->setItem(row, 2, contextItem);
            row++;
        }
        file.close();
    }
}

/**
 * @brief HmiLog::ProcessRawLog 处理原始数据
 * @param[in] rawStr
 * @param[out] time 时间
 * @param[out] type 日志类型
 * @param[out] context  日志内容
 * @return
 */
Bit32 HmiLog::ProcessRawLog(QString rawStr, QString &time, QString &type, QString &context)
{
    if (rawStr.isEmpty())
    {
        return -1;
    }
    QRegularExpression re ("^\\[(?<time>.*)\\]\\((?<type>\\w)\\):(?<context>.*)$");
    QRegularExpressionMatch match = re.match(rawStr);
    if (match.hasMatch())
    {
        time = match.captured("time");
        type = match.captured("type");
        context = match.captured("context");
    }
    return 0;
}
