#ifndef COMMON_H
#define COMMON_H
#include <QTextCodec>
#include <QString>
#include <QStatusBar>
#include <QLayout>
#include <QCoreApplication>

#include "datadef.h"

#define UTF8 QTextCodec::codecForName("UTF-8")
#define TR(str) tr((UTF8->fromUnicode(str).data()))
#define UTF_8 "UTF-8"
#define FONT_STYLE "Microsoft YaHei UI"

inline constexpr Bit32 WINDOW_WIDTH =  1600;
inline constexpr Bit32 WINDOW_HEIGHT = 900;
    
inline constexpr Bit32 DLG_WIDTH =  400;   // 对话框宽度
inline constexpr Bit32 DLG_HEIGHT = 300;   // 对话框高度

#ifdef QT_DEBUG // 测试环境
// 输出到控制台
void ComDebug(const QString &content, const QString &title = "");
void ComDebug(const Bit16 &content, const QString &title = "");
void ComDebug(const Bit32 &content, const QString &title = "");
void ComDebug(const Bit64 &content, const QString &title = "");
void ComDebug(const fBit32 &content, const QString &title = "");
void ComDebug(const fBit64 &content, const QString &title = "");
void ComDebug(const QStringList content, const QString &title = "");
#endif

// 状态栏提示
void InitStatusBar(QStatusBar *statusBar); // 初始化状态栏
void DestoryStatusBar(); // 销毁状态栏
void PromptOut(const QString &content, Bit32 outtime = 3000);

// 获取数据的范围
QPair<fBit64, fBit64> GetRange(const QVector<fBit64> &in);

bool LayoutContainsWidget(QLayout *layout, QWidget* widget);
#endif // COMMON_H
