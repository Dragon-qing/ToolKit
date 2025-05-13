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
    QString TransLogType2Str(Bit32 type); // 转换日志类型

};

#endif // HMILOG_H
