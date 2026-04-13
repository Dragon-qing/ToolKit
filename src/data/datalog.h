#ifndef DATALOG_H
#define DATALOG_H

#include <QStandardItemModel>

#include "common.h"

class DataLog
{
public:
    static DataLog &GetInstance();
    DataLog();

    void LoadLog(QStandardItemModel *model);
private:
    QString TransLogType2Str(Bit32 type); // 转换日志类型

};

#endif // DATALOG_H
