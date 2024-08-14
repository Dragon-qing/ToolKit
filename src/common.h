#ifndef COMMON_H
#define COMMON_H
#include <QTextCodec>
#include <QString>

#include "datadef.h"
#include "logdt.h"

#define UTF8 QTextCodec::codecForName("UTF-8")
#define TR(str) tr((UTF8->fromUnicode(str).data()))

#define TOOLS_PATH "./tools"

const Bit32 WINDOW_WIDTH =  800;
const Bit32 WINDOW_HEIGHT = 600;

void ComDebug(const QString &content, const QString &title = "");
void ComDebug(const QStringList content, const QString &title = "");
#endif // COMMON_H
