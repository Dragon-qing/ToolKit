#ifndef WGBTFMAKE_H
#define WGBTFMAKE_H

#include <QWidget>
#include <QLabel>
#include <QTimer>

#include "datadef.h"

#define BTF_INFO_CLR_MAX_TIME (2000)

namespace Ui {
class WgBTFMake;
}

class WgBTFMake : public QWidget
{
    Q_OBJECT

public:
    explicit WgBTFMake(QWidget *parent = nullptr);
    ~WgBTFMake();
    bool eventFilter(QObject *obj, QEvent *event) override;

public slots:
    void on_selectDirBtn_clicked();
    void on_clsBtn_clicked();
    void on_startBtn_clicked();
// QWidget interface
protected:
    void dragEnterEvent(QDragEnterEvent *event) override;
    void dropEvent(QDropEvent *event) override;


private:
    Ui::WgBTFMake *ui;
    QString GetImage(const QString &path);
    QList<QLabel*> m_labelList;
    QStringList m_nameList;
    QStringList m_pathList;
    QTimer *m_timer;

    void ClearList();
    void RemoveItem(QLabel *label);
    void RefreshWidget();
    void SetTip(const QString &tipStr, bool isAutoClear = false, Bit32 mes = BTF_INFO_CLR_MAX_TIME);

private slots:
    void TimeoutHandler();

};

#endif // WGBTFMAKE_H
