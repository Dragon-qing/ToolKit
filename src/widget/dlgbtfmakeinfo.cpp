#include "dlgbtfmakeinfo.h"
#include "ui_dlgbtfmakeinfo.h"

DlgBTFMakeInfo::DlgBTFMakeInfo(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::DlgBTFMakeInfo)
{
    ui->setupUi(this);
}

DlgBTFMakeInfo::~DlgBTFMakeInfo()
{
    delete ui;
}
