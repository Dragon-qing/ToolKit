#ifndef BASEWIDGET_H
#define BASEWIDGET_H

#include <QWidget>

#include "common.h"
#include "msgdata.h"

namespace Ui {
class BaseWidget;
}

class BaseWidget : public QWidget
{
    Q_OBJECT

public:
    explicit BaseWidget(QWidget *parent = nullptr);
    ~BaseWidget();
    virtual void MessageQueue(QVariant messageid, QVariant messageValue);
    virtual QStringList GetHelpText();

private:
    Ui::BaseWidget *ui;
};

#endif // BASEWIDGET_H
