#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include "datadef.h"

#include "widgetmanager.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    // QObject interface
protected:
    void timerEvent(QTimerEvent *event);

private:
    Ui::MainWindow *ui;
    WidgetManager *m_pWidgetManager;
    Bit32 m_nTimer1;    // 200ms定时器
    Bit32 m_nTimer2;    // 20ms定时器

    void RegistStruct(); // 注册自定义结构体
    void RegistUI(); // 注册UI
};
#endif // MAINWINDOW_H
