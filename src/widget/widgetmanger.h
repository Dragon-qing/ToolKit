#ifndef WIDGETMANGER_H
#define WIDGETMANGER_H

#include <QWidget>

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

    void InitWidgetContainer();
    void AddWidget(QWidget *widget, const QString &name);
private slots:
    void WidgetChangeHandle(int index);
};

#endif // WIDGETMANGER_H
