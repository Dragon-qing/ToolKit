#include <QDir>

#include <QDebug>
#include <QDateTime>

#include "sysapi.h"

#include "common.h"

QStatusBar *GLOBAL_STATUSBAR = NULL;

# ifdef QT_DEBUG
void ComDebug(const QString &content, const QString &title)
{
    qDebug() << QString("<------------%1------------>").arg(title);
    qDebug() << QDateTime::currentDateTime().toString("[yy-MM-dd hh:mm:ss]");
    qDebug() << content;
    qDebug() << QString(">------------------------<");
}

void ComDebug(const Bit16 &content, const QString &title) { ComDebug(QString::number(content), title); }

void ComDebug(const Bit32 &content, const QString &title) { ComDebug(QString::number(content), title); }

void ComDebug(const Bit64 &content, const QString &title) { ComDebug(QString::number(content), title); }

void ComDebug(const fBit32 &content, const QString &title) { ComDebug(QString::number(content), title); }

void ComDebug(const fBit64 &content, const QString &title) { ComDebug(QString::number(content), title); }

void ComDebug(const QStringList content, const QString &title)
{
    qDebug() << QString("<------------%1------------>").arg(title);
    qDebug() << QDateTime::currentDateTime().toString("[yy-MM-dd hh:mm:ss]");
    foreach (QString str, content)
    {
        qDebug() << str;
    }
    qDebug() << QString(">------------------------<");
}
# endif

/**
 * @brief GetXmlDoc 获取xml对象
 * @param[in] path 文件路径
 * @param[out] doc 输出对象
 * @return 0：成功; <0:失败
 */
Bit32 GetXmlDoc(QString path, QDomDocument &doc)
{
    if (path.isEmpty())
    {
        return -1;
    }

    QString errorMsg("");
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
    if (!errorMsg.isEmpty())
    {
        TKLogger::Instance().AddLog(WARNING_LOG, errorMsg);
    }

    file.close();
    return 0;
}

/**
 * @brief SaveXmlDoc 保存xml对象到指定文件路径
 * @param path 要保存的文件路径
 * @param doc xml对象
 * @return 0：成功，-1：失败
 */
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

/**
 * @brief StrToQByte 按指定编码转换字符串
 * @param str 待转换的字符串
 * @param unicode 编码格式：UTF-8、GBK、Shift-JIS 等。
 * @return
 */
QByteArray StrToQByte(QString str, const Bit8 *unicode)
{
    QTextCodec *codec = QTextCodec::codecForName(unicode);
    QByteArray bytearr;
    bytearr = codec->fromUnicode(str);
    return bytearr;
}

void InitStatusBar(QStatusBar *statusBar)
{
    GLOBAL_STATUSBAR = statusBar;
}

void DestoryStatusBar()
{
    GLOBAL_STATUSBAR = nullptr;
}

/**
 * @brief PromptOut 设置下方状态栏文本
 * @param content 文本内容
 * @param outtime 多久后自动清除（0：为不清除）ms
 */
void PromptOut(const QString &content, Bit32 outtime)
{
    if (GLOBAL_STATUSBAR == nullptr)
    {
        return;
    }

    GLOBAL_STATUSBAR->showMessage(content, outtime);
}

/**
 * @brief GetRange 获取范围
 * @note QVector<fBit64>数组的范围
 * @param in
 * @return
 */
QPair<fBit64, fBit64> GetRange(const QVector<fBit64> &in)
{
    if (in.isEmpty())
    {
        return QPair<fBit64, fBit64>();
    }

    fBit64 min = in.first();
    fBit64 max = min;
    foreach (fBit64 v, in)
    {
        min = qMin(v, min);
        max = qMax(v, max);
    }

    return QPair<fBit64, fBit64>(min, max);
}

/**
 * @brief LayoutContainsWidget 判断layout中是否存在指定widget
 * @param layout
 * @param widget
 * @return
 */
bool LayoutContainsWidget(QLayout *layout, QWidget *widget)
{
    for (Bit32 i = 0; i < layout->count(); ++i)
    {
        QLayoutItem *item = layout->itemAt(i);
        if (item && item->widget() == widget)
        {
            return true;
        }
    }

    return false;
}

QString GetExePath()
{
    return QCoreApplication::applicationDirPath();
}
