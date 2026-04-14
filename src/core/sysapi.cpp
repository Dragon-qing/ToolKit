/*!
 * @brief 系统路径接口模块
 * @file sysapi.cpp
 * @author Dragon_qing
 * @date 2025/04/27
 */
#include <QDir>

#include "sysapi.h"
#include "common.h"


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
        path = "../logs";
        break;
    case SCRIPT_PATH:
        path = "../script";
        break;
    case PLUGINS_PATH:
        path = "../plugins";
        break;
    default:
        break;
    }
    path = QDir::cleanPath(QDir(QCoreApplication::applicationDirPath()).filePath(path));
    return QDir::toNativeSeparators(path);
}
