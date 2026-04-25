/*
 * @Author: Dragon-qing
 * @Date: 2026-04-18
 * @LastEditors: Dragon-qing
 * @FilePath: \ToolKit\src\utils\xmlopt.h
 * @Description: xml工具
 */
#ifndef XMLOPT_H
#define XMLOPT_H
#include <QDomDocument>

#include "datadef.h"

/**
 * @brief: 获取xml对象
 * @param {QString} path: xml文件路径
 * @param {QDomDocument} &doc: 输出xml对象
 * @param {QString} &errorMsg: 错误信息
 * @return {Bit32} 0：成功; <0:失败
 */
Bit32 GetXmlDoc(QString path, QDomDocument &doc, QString &errorMsg);

/**
 * @brief: 保存xml对象到指定文件路径
 * @param {QString} path: xml文件路径
 * @param {QDomDocument} &doc: 要保存的xml对象
 * @return {Bit32} 0：成功; <0:失败
 */
Bit32 SaveXmlDoc(QString path, const QDomDocument &doc);

#endif