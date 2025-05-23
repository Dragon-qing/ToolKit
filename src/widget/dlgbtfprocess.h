#ifndef DLGBTFPROCESS_H
#define DLGBTFPROCESS_H

#include <QDialog>
#include <QTimer>

namespace Ui {
class DlgBtfProcess;
}

class DlgBtfProcess : public QDialog
{
    Q_OBJECT

public:
    explicit DlgBtfProcess(QWidget *parent = nullptr);
    ~DlgBtfProcess();
    void paintEvent(QPaintEvent *event) override;
    void Start();
public slots:
    void Done();
    void Faild();
private:
    Ui::DlgBtfProcess *ui;
    QTimer *m_timer;
private slots:
    void TimeOutHandle();
    void on_okBtn_clicked();
};

#endif // DLGBTFPROCESS_H
