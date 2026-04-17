#ifndef WGARCHIVEMAKER_H
#define WGARCHIVEMAKER_H

#include <QLabel>
#include <QTimer>
#include <QThread>

#include "datadef.h"
#include "basewidget.h"
#include "exeexternaltool.h"

#include "dlgarchivemakeinfo.h"
#include "dlgarchiveprocess.h"

#define TIPS_INFO_CLR_MAX_TIME (3000)
#define MAX_DISITEM_NUM (6)

namespace Ui {
class WgArchiveMaker;
}

class WgArchiveMaker : public BaseWidget
{
    Q_OBJECT

public:
    explicit WgArchiveMaker(QWidget *parent = nullptr);
    ~WgArchiveMaker();
    bool eventFilter(QObject *obj, QEvent *event) override;

public slots:
    void on_selectDirBtn_clicked();
    void on_clsBtn_clicked();
    void on_startBtn_clicked();

protected:
    void dragEnterEvent(QDragEnterEvent *event) override;
    void dropEvent(QDropEvent *event) override;
    void paintEvent(QPaintEvent *event) override;

private:
    Ui::WgArchiveMaker *ui;
    QList<QLabel*> m_labelList;
    QStringList m_nameList;
    QStringList m_pathList;
    QTimer *m_pTimer; // 计时器，用于定时清除提示信息
    DlgArchiveMakeInfo *m_pDlgInfo;
    QLabel *m_pMoreLabel; // 用于显示“更多”提示的标签
    bool m_bHasMoreLabel;
    DlgArchiveProcess *m_pDlgProcess;
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
    void SetTip(const QString &tipStr, bool isAutoClear = false, Bit32 mes = TIPS_INFO_CLR_MAX_TIME);
    bool ContainsWidget(QLabel *label);

private slots:
    void TimeoutHandler();
};

#endif // WGARCHIVEMAKER_H
