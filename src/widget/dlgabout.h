#ifndef DLGABOUT_H
#define DLGABOUT_H

#include <QDialog>
#include <QScopedPointer>

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
    QScopedPointer<DlgAboutPrivate> d_ptr; // d指针 QScopedPointer类似cpp的智能指针unique_ptr
    Q_DECLARE_PRIVATE(DlgAbout)
    
    /**
     * @brief: 初始化
     * @return {*}
     */
    void Init();

    /**
     * @brief: 刷新
     * @return {*}
     */
    void Refresh();
};

#endif // DLGABOUT_H
