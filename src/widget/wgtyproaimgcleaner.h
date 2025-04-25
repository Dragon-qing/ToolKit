#ifndef WGTYPROAIMGCLEANER_H
#define WGTYPROAIMGCLEANER_H

#include "basewidget.h"

namespace Ui {
class WgTyproaImgCleaner;
}

class WgTyproaImgCleaner : public BaseWidget
{
    Q_OBJECT

public:
    explicit WgTyproaImgCleaner(QWidget *parent = nullptr);
    ~WgTyproaImgCleaner();

private:
    Ui::WgTyproaImgCleaner *ui;
};

#endif // WGTYPROAIMGCLEANER_H
