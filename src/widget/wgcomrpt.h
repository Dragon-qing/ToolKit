#ifndef WGCOMRPT_H
#define WGCOMRPT_H

#include <QWidget>

namespace Ui {
class WgComRpt;
}

class WgComRpt : public QWidget
{
    Q_OBJECT

public:
    explicit WgComRpt(QWidget *parent = nullptr);
    ~WgComRpt();

private:
    Ui::WgComRpt *ui;
};

#endif // WGCOMRPT_H
