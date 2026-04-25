#ifndef SYSAPI_H
#define SYSAPI_H
#include <QString>

#include "datadef.h"

#define SYS_VERSION "0.3.3"

typedef enum _Path_Type{
    BIN_PATH = 0,   // bin目录
    TMP_PATH,       // tmp目录
    DATA_PATH,      // data目录
    LOG_PATH,       // log目录
    SCRIPT_PATH,    // script目录
    PLUGINS_PATH,   // plugins目录
    TOOLS_PATH,     // tools目录

    PATH_NUM
}PATH_TYPE;

/**
 * @brief: 获取系统路径
 * @param {PATH_TYPE} type: 路径类型
 * @return {QString} 路径字符串
 */
QString GetSysPath(PATH_TYPE type);

/**
 * @brief: 获取可执行文件路径
 * @return {QString} 路径字符串
 */
QString GetExePath();
#endif // SYSAPI_H
