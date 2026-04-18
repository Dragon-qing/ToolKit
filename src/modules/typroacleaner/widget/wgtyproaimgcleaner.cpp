#include "wgtyproaimgcleaner.h"
#include "ui_wgtyproaimgcleaner.h"

WgTyproaImgCleaner::WgTyproaImgCleaner(QWidget *parent) :
    BaseWidget(parent),
    ui(new Ui::WgTyproaImgCleaner)
{
    ui->setupUi(this);
}

WgTyproaImgCleaner::~WgTyproaImgCleaner()
{
    delete ui;
}
