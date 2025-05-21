#include "basewidget.h"
#include "ui_basewidget.h"

BaseWidget::BaseWidget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::BaseWidget)
{
    ui->setupUi(this);
}

BaseWidget::~BaseWidget()
{
    delete ui;
}

void BaseWidget::MessageQueue(QVariant messageid, QVariant messageValue)
{
    Q_UNUSED(messageid);
    Q_UNUSED(messageValue);

    return;
}

QStringList BaseWidget::GetHelpText()
{
    return QStringList();
}
