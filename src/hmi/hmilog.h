#ifndef HMILOG_H
#define HMILOG_H

#include <QStandardItemModel>

#include "common.h"

class HmiLog
{
public:
    static HmiLog &GetInstance();
    HmiLog();

    void LoadLog(QStandardItemModel *model);
private:
    Bit32 ProcessRawLog(QString rawStr, QString &time, QString &type, QString &context);

};

#endif // HMILOG_H
