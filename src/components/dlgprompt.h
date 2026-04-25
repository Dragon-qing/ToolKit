#ifndef DLGPROMPT_H
#define DLGPROMPT_H

#include <QDialog>
#include <QButtonGroup>

#include "common.h"

namespace Ui {
class DlgPrompt;
}

class DlgPrompt : public QDialog
{
    Q_OBJECT

public:
    enum _DLG_CODE{     // 返回结果
        OK_CODE = 0,
        CANCEL_CODE,

        CODE_NUM
    };
    enum _BUTTON_TYPE{
        OK_BUTTON = 1,  // 确认按钮
        CANCEL_BUTTON = 2,  // 取消按钮

        TYEP_NUM
    };
    explicit DlgPrompt(Bit32 buttons = (OK_BUTTON | CANCEL_BUTTON), QWidget *parent = nullptr);
    ~DlgPrompt();
    Bit32 ExecAndRet(const QString &context);

    /**
     * @brief: 设置标题
     * @param {QString} &title: 
     * @return {*}
     */
    void SetTitle(const QString &title);
protected:
    void paintEvent(QPaintEvent *event) override;

private:
    Ui::DlgPrompt *ui;
    QButtonGroup *m_pButtonGroup;
    Bit32 m_nCurrentCode;   // 当前结果代码

    void InitUi();
    void HandleClicked(int id);
};

#endif // DLGPROMPT_H
