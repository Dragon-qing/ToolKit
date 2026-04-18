/*
 * @Author: Dragon-qing
 * @Date: 2026-04-18
 * @LastEditors: Dragon-qing
 * @FilePath: \ToolKit\src\utils\stringopt.cpp
 * @Description: 字符串工具
 */
#include "stringopt.h"

QByteArray StrToQByte(QString str, const Bit8 *unicode)
{
    QTextCodec *codec = QTextCodec::codecForName(unicode);
    QByteArray bytearr;
    bytearr = codec->fromUnicode(str);
    return bytearr;
}

QString QByteToStr(QByteArray bytearr, const Bit8 *unicode)
{
    QTextCodec *codec = QTextCodec::codecForName(unicode);
    QString str;
    str = codec->toUnicode(bytearr);
    return str;
}
