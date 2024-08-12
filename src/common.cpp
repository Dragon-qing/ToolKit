#include <QDebug>
#include <QDateTime>

#include "common.h"


void ComDebug(const QString &content, const QString &title)
{
    qDebug() << QString("------------%1------------").arg(title);
    qDebug() << content;
    qDebug() << QDateTime::currentDateTime().toString("[yy-MM-dd hh:mm:ss]");
    qDebug() << QString("------------------------");
}

void ComDebug(const QStringList content, const QString &title)
{
    qDebug() << QString("------------%1------------").arg(title);
    foreach (QString str, content)
    {
        qDebug() << str;
    }
    qDebug() << QDateTime::currentDateTime().toString("[yy-MM-dd hh:mm:ss]");
    qDebug() << QString("------------------------");
}
