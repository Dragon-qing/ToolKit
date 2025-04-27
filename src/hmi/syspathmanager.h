#ifndef SYSPATHMANAGER_H
#define SYSPATHMANAGER_H
#include <QString>

#include "datadef.h"

typedef enum _Path_Type{
    BIN_PATH = 0,   // bin目录
    TMP_PATH,       // tmp目录
    DATA_PATH,      // data目录
    LOG_PATH,       // log目录
    SCRIPT_PATH,    // script目录

    PATH_NUM
}PATH_TYPE;

QString GetSysPath(PATH_TYPE type);

#endif // SYSPATHMANAGER_H
