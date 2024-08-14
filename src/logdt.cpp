#include <QDebug>
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
}

Bit32 LogDt::AddLog(Bit32 type, QString logStr)
{
    if (type <= DEBUG_LOG || type >= TYPE_NUM)
    {
        return -1;
    }
    switch (type)
    {
    case DEBUG_LOG:
        qDebug() << logStr;
        break;
    case INFO_LOG:
        qInfo() << logStr;
        break;
    case WARNING_LOG:
        qWarning() << logStr;
        break;
    case CRITICAL_LOG:
        qCritical() << logStr;
        break;
    case FATAL_LOG:
        qFatal(logStr.toLatin1().data());
        break;
    default:
        break;
    }

    return 0;
}

