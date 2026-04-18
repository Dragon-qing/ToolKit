/*
 * @Author: Dragon-qing
 * @Date: 2026-04-18
 * @LastEditors: Dragon-qing
 * @FilePath: \ToolKit\src\utils\stringopt.h
 * @Description: 字符串工具
 */
#ifndef STRINGOPT_H
#define STRINGOPT_H

#include <QByteArray>
#include <QString>
#include <QTextCodec>

#include "datadef.h"

/**
 * @brief StrToQByte 按指定编码转换字符串为QByteArray
 * @param str 待转换的字符串
 * @param unicode 编码格式：UTF-8、GBK、Shift-JIS 等。
 * @return 转换后的QByteArray
 */
QByteArray StrToQByte(QString str, const Bit8 *unicode); // QString按指定编码转换为QByteArray

/**
 * @brief: QByteArrya按指定编码转换为QString
 * @param {QByteArray} bytearr: 待转换的QByteArray
 * @param {Bit8} *unicode: 编码格式：UTF-8、GBK、Shift-JIS 等。
 * @return {QString} 转换后的QString
 */
QString QByteToStr(QByteArray bytearr, const Bit8 *unicode);

#endif