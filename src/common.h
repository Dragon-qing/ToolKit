#ifndef COMMON_H
#define COMMON_H
#include <QTextCodec>
#include <QString>
#include <QDomDocument>
#include <QStatusBar>

#include "datadef.h"
#include "logdt.h"

#define UTF8 QTextCodec::codecForName("UTF-8")
#define TR(str) tr((UTF8->fromUnicode(str).data()))
#define UTF_8 "UTF-8"
#define FONT_STYLE "Microsoft YaHei"

#define TOOLS_PATH "./tools"

const Bit32 WINDOW_WIDTH =  1024;
const Bit32 WINDOW_HEIGHT = 768;

const Bit32 DLG_WIDTH =  400;   // 对话框宽度
const Bit32 DLG_HEIGHT = 200;   // 对话框高度

void ComDebug(const QString &content, const QString &title = "");
void ComDebug(const QStringList content, const QString &title = "");

Bit32 GetXmlDoc(QString path, QDomDocument &doc);
Bit32 SaveXmlDoc(QString path, const QDomDocument &doc);

QByteArray StrToQByte(QString str, const Bit8 *unicode); // QString按指定编码转换为QByte

void InitStatusBar(QStatusBar *statusBar);
void PromptOut(const QString &content, Bit32 outtime = 0);
// 获取数据的范围
QPair<fBit64, fBit64> GetRange(const QVector<fBit64> &in);
#endif // COMMON_H
