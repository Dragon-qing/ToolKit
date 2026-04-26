#include <QDir>

#include <QDebug>
#include <QDateTime>
#include <QSvgRenderer>
#include <QPainter>

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

QPixmap RenderSvg(const QString& path, int w, int h) 
{
    QSvgRenderer renderer(path);
    QPixmap pix(w, h);
    pix.fill(Qt::transparent);

    QPainter p(&pix);
    renderer.render(&p);
    return pix;
}