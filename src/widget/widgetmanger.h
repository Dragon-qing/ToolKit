#ifndef WIDGETMANGER_H
#define WIDGETMANGER_H

#include <QWidget>

#include "basewidget.h"

#include "dlghelp.h"

namespace Ui {
class WidgetManger;
}

class WidgetManger : public QWidget
{
    Q_OBJECT

public:
    explicit WidgetManger(QWidget *parent = nullptr);
    ~WidgetManger();
    QWidget *GetCurrentWidget();

private:
    Ui::WidgetManger *ui;

    QList<QWidget *> m_widgetContainer;
    QStringList m_widgetNameList;
    DlgHelp *m_pDlgHelp;

    void InitWidgetContainer();
    void AddWidget(QWidget *widget, const QString &name);
private slots:
    void WidgetChangeHandle(int index);
    void CallHelpDlg();

};

#endif // WIDGETMANGER_H
