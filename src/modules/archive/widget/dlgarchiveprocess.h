#ifndef DLGARCHIVEPROCESS_H
#define DLGARCHIVEPROCESS_H

#include <QDialog>
#include <QTimer>

#include "externaltoolbase.h"

namespace Ui {
class DlgArchiveProcess;
}

class DlgArchiveProcess : public QDialog
{
    Q_OBJECT

public:
    explicit DlgArchiveProcess(QWidget *parent = nullptr);
    ~DlgArchiveProcess();
    void paintEvent(QPaintEvent *event) override;

    /**
     * @brief: 重置界面状态
     * @return {}
     */
    void ReSet();
    void SetExternalTool(ExternalToolBase *pExternalTool); // 设置外部工具对象指针

public slots:
    void Done();
    void Faild();
    void OnValueChanged(int value);

private:
    Ui::DlgArchiveProcess *ui;
    QTimer *m_timer;
    ExternalToolBase *m_pExternalTool; // 用于管理外部工具的进程
private slots:
    void TimeOutHandle(); // 处理界面...动画
    void on_okBtn_clicked();
    void on_cancelBtn_clicked();
};

#endif // DLGARCHIVEPROCESS_H
