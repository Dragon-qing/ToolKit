#ifndef LOGDT_H
#define LOGDT_H
#include <QMap>

#include "datadef.h"


class LogDt
{
public:
    enum LogDataType{
        INVALID_LOG_TYPE = -1,
        SYSTEM_ERROR_LOG_TYPE = 0,
        GENERAL_LOG_TYPE,
        FILE_LOG_TYPE,

        TYPE_NUM
    };
    static LogDt& Instance();
    ~LogDt();

    Bit32 InsertLog(Bit32 type, QString logStr);
private:
    explicit LogDt();

    QMap<Bit32, QString> m_logStrList;
};

#endif // LOGDT_H
