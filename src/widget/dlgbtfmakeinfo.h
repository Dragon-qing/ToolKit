#ifndef DLGBTFMAKEINFO_H
#define DLGBTFMAKEINFO_H

#include <QDialog>

namespace Ui {
class DlgBTFMakeInfo;
}

class DlgBTFMakeInfo : public QDialog
{
    Q_OBJECT

public:
    explicit DlgBTFMakeInfo(QWidget *parent = nullptr);
    ~DlgBTFMakeInfo();

private:
    Ui::DlgBTFMakeInfo *ui;
};

#endif // DLGBTFMAKEINFO_H
