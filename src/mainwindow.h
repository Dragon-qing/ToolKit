#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include "datadef.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    enum{
        GENERATE_BUG_FOLDER = 0,
        TYPROA_IMG_CLEANER,

        WIDGET_NUM
    };
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void SetCurrentWidget(Bit32 idx);

public slots:

private:
    Ui::MainWindow *ui;
    QList<QWidget*> m_widgetContainer;
    Bit32 m_nCurrentWidgetIdx;

    void InerstWidget(QWidget *widget, QAction *action, Bit32 widgetType);
};
#endif // MAINWINDOW_H
