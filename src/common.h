#ifndef COMMON_H
#define COMMON_H
#include <QTextCodec>

#include "datadef.h"
#include "logdt.h"

#define UTF8 QTextCodec::codecForName("UTF-8")
#define TR(str) tr((UTF8->fromUnicode(str).data()))

const Bit32 WINDOW_WIDTH =  800;
const Bit32 WINDOW_HEIGHT = 600;

#endif // COMMON_H
