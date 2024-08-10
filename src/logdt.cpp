#include "logdt.h"

LogDt &LogDt::Instance()
{
    static LogDt s_data;
    return s_data;
}

LogDt::~LogDt()
{

}

LogDt::LogDt()
{
    m_logStrList.clear();
}

Bit32 LogDt::InsertLog(Bit32 type, QString logStr)
{
    if (type <= INVALID_LOG_TYPE || type >= TYPE_NUM)
    {
        return -1;
    }
    m_logStrList.insert(type, logStr);

    return 0;
}

