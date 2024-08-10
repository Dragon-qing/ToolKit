#ifndef WGTYPROAIMGCLEANER_H
#define WGTYPROAIMGCLEANER_H

#include <QWidget>

namespace Ui {
class WgTyproaImgCleaner;
}

class WgTyproaImgCleaner : public QWidget
{
    Q_OBJECT

public:
    explicit WgTyproaImgCleaner(QWidget *parent = nullptr);
    ~WgTyproaImgCleaner();

private:
    Ui::WgTyproaImgCleaner *ui;
};

#endif // WGTYPROAIMGCLEANER_H
