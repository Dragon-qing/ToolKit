#ifndef LOGDT_H
#define LOGDT_H
#include <QMap>

#include "datadef.h"


enum LogDataType{
    DEBUG_LOG = 0,
    INFO_LOG,
    WARNING_LOG,
    CRITICAL_LOG,
    FATAL_LOG,

    TYPE_NUM
};

class LogDt
{
public:
    static LogDt& Instance();
    ~LogDt();

    Bit32 AddLog(Bit32 type, QString logStr);
private:
    explicit LogDt();
};

#endif // LOGDT_H
