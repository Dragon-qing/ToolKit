#ifndef DLGABOUT_H
#define DLGABOUT_H

#include <QDialog>

namespace Ui {
class DlgAbout;
}

class DlgAboutPrivate;

class DlgAbout : public QDialog
{
    Q_OBJECT
    
public:
    explicit DlgAbout(QWidget *parent = nullptr);
    ~DlgAbout();
    
private:
    Ui::DlgAbout *ui;
    
    // 初始化界面
    void Init();

private slots:
    void on_pushButton_close_clicked();
};

#endif // DLGABOUT_H
