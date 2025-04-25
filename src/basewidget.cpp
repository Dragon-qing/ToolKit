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

void BaseWidget::MassageQueue(QVariant messageid, QVariant messageValue)
{
    Q_UNUSED(messageid);
    Q_UNUSED(messageValue);

    return;
}
