#ifndef WGBTFMAKE_H
#define WGBTFMAKE_H

#include <QLabel>
#include <QTimer>
#include <QThread>

#include "datadef.h"
#include "basewidget.h"
#include "exeexternaltool.h"

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
    QTimer *m_pTimer; // 计时器，用于定时清除提示信息
    DlgBTFMakeInfo *m_pDlgInfo;
    QLabel *m_pMoreLabel; // 用于显示“更多”提示的标签
    bool m_bHasMoreLabel;
    DlgBtfProcess *m_pDlgProcess;
    BTFProcessThread *m_pThread;
    std::unique_ptr<ExternalToolBase> m_p7zTool; // 7z工具实例指针

    /**
     * @brief: 根据内容创建一个图像标签
     * @param {QString} &path: 文件路径或文件夹路径，特殊值"more"表示创建一个“更多”标签
     * @return {QLabel *}
     */
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
