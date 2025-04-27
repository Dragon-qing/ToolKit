/*!
 * @brief 系统路径接口模块
 * @file syspathmanager.cpp
 * @author Dragon_qing
 * @date 2025/04/27
 */
#include <QDir>

#include "syspathmanager.h"


QString GetSysPath(_Path_Type type)
{
    QString path = "";
    switch (type)
    {
    case BIN_PATH:
        path = "../bin";
        break;
    case DATA_PATH:
        path = "../data";
        break;
    case TMP_PATH:
        path = "../tmp";
        break;
    case LOG_PATH:
        path = "../log";
        break;
    case SCRIPT_PATH:
        path = "../script";
        break;
    default:
        break;
    }

    return QDir::toNativeSeparators(path);
}
