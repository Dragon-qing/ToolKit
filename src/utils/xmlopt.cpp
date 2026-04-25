/*
 * @Author: Dragon-qing
 * @Date: 2026-04-18
 * @LastEditors: Dragon-qing
 * @FilePath: \ToolKit\src\utils\xmlopt.cpp
 * @Description: xml工具
 */
#include <QDir>
#include <QFile>
#include <QTextStream>

#include "common.h"

#include "xmlopt.h"

Bit32 GetXmlDoc(QString path, QDomDocument &doc, QString &errorMsg)
{
    if (path.isEmpty())
    {
        return -1;
    }

    Bit32 errorLine = 0;
    Bit32 errorColumn = 0;
    QString absPath = QDir(path).absolutePath();

    QFile file(absPath);
    if (!file.exists())
    {
        errorMsg = QObject::TR("common:%1文件不存在").arg(absPath);
        return -1;
    }
    if (!file.open(QFile::ReadOnly | QFile::Text))
    {
        errorMsg = QObject::TR("common:%1文件打开失败").arg(absPath);
        return -2;
    }
    if (!doc.setContent(&file, false, &errorMsg, &errorLine, &errorColumn))
    {
        errorMsg = QObject::TR("common:%1文件格式错误,Msg:%2,line:%3,column:%4").arg(absPath)
                       .arg(errorMsg).arg(errorLine).arg(errorColumn);
        return -3;
    }

    file.close();
    return 0;
}

Bit32 SaveXmlDoc(QString path, const QDomDocument &doc)
{
    if (path.isEmpty())
    {
        return -1;
    }

    QString filePath = "";
    filePath = QDir::toNativeSeparators(path);

    QFile file(filePath);
    if (!file.open(QFile::WriteOnly | QFile::Truncate))
    {
        return -1;
    }

    QTextStream textStream(&file);
    textStream.setCodec(UTF_8);
    textStream.reset();
    doc.save(textStream, 4, QDomNode::EncodingFromTextStream);

    file.flush();
    file.close();
    return 0;
}