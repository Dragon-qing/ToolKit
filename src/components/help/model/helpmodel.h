#ifndef HELPMODEL_H
#define HELPMODEL_H
#include <QStringList>

struct HelpContentDTO
{
    QString category; // 帮助分类
    QStringList content;  // 帮助内容
};

#endif // HELPMODEL_H