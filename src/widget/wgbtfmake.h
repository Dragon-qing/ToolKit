#ifndef WGBTFMAKE_H
#define WGBTFMAKE_H

#include <QLabel>
#include <QTimer>
#include <QThread>

#include "basewidget.h"
#include "datadef.h"

#include "dlgbtfmakeinfo.h"
#include "dlgbtfprocess.h"

#define BTF_INFO_CLR_MAX_TIME (3000)
#define MAX_DISITEM_NUM (6)

class BTFProcessThread;

namespace Ui {
class WgBTFMake;
}

class WgBTFMake : public BaseWidget
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
    void paintEvent(QPaintEvent *event) override;

private:
    Ui::WgBTFMake *ui;
    QList<QLabel*> m_labelList;
    QStringList m_nameList;
    QStringList m_pathList;
    QTimer *m_pTimer;
    DlgBTFMakeInfo *m_pDlgInfo;
    QLabel *m_pMoreLabel;
    bool m_bHasMoreLabel;
    DlgBtfProcess *m_pDlgProcess;
    BTFProcessThread *m_pThread;

    QLabel *CreateImgLabel(const QString &path);
    void ClearList();
    void RemoveItem(QLabel *label);
    void Refresh();
    void SetTip(const QString &tipStr, bool isAutoClear = false, Bit32 mes = BTF_INFO_CLR_MAX_TIME);
    bool ContainsWidget(QLabel *label);

private slots:
    void TimeoutHandler();
};

class BTFProcessThread : public QThread
{
    Q_OBJECT
public:
    explicit BTFProcessThread(QWidget *parent = nullptr);
    ~BTFProcessThread();
    void SetConfig(const QStringList &fileList, const QString &saveName);

signals:
    void MakeDone();
    void MakeFaild();

protected:
    void run() override;

private:
    QStringList m_fileList; // 文件列表
    QString m_sSaveName;    // 保存名字
};

#endif // WGBTFMAKE_H
