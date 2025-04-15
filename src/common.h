#ifndef COMMON_H
#define COMMON_H
#include <QTextCodec>
#include <QString>
#include <QDomDocument>

#include "datadef.h"
#include "logdt.h"

#define UTF8 QTextCodec::codecForName("UTF-8")
#define TR(str) tr((UTF8->fromUnicode(str).data()))
#define UTF_8 "UTF-8"

#define TOOLS_PATH "./tools"

const Bit32 WINDOW_WIDTH =  800;
const Bit32 WINDOW_HEIGHT = 600;

void ComDebug(const QString &content, const QString &title = "");
void ComDebug(const QStringList content, const QString &title = "");

Bit32 GetXmlDoc(QString path, QDomDocument &doc);
Bit32 SaveXmlDoc(QString path, const QDomDocument &doc);
#endif // COMMON_H
