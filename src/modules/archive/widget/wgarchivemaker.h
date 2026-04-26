#ifndef WGARCHIVEMAKER_H
#define WGARCHIVEMAKER_H

#include <QLabel>

#include "datadef.h"
#include "basewidget.h"
#include "exeexternaltool.h"

#include "dlgarchivemakeinfo.h"
#include "dlgarchiveprocess.h"
#include "archiveservice.h"
#include "overlay.h"

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

protected:
    void dragEnterEvent(QDragEnterEvent *event) override;
    void dragLeaveEvent(QDragLeaveEvent *event) override;
    void dropEvent(QDropEvent *event) override;
    void paintEvent(QPaintEvent *event) override;

    void resizeEvent(QResizeEvent *event) override;

private:
    Ui::WgArchiveMaker *ui;
    QList<QLabel*> m_labelList;

    DlgArchiveMakeInfo *m_pDlgInfo;
    QLabel *m_pMoreLabel; // 用于显示“更多”提示的标签
    bool m_bHasMoreLabel;
    DlgArchiveProcess *m_pDlgProcess;
    ArchiveService m_archiveService; // 用于管理压缩相关的服务和工具
    Overlay *m_pOverlay; // 用于显示操作提示的遮罩组件
    
    /**
     * @brief: 根据内容创建一个图像标签
     * @param {QString} &path: 文件路径或文件夹路径，特殊值"more"表示创建一个“更多”标签
     * @return {QLabel *}
     */
    QLabel *CreateImgLabel(const QString &path);
    void ClearList();
    void RemoveItem(QLabel *label);
    void Refresh();
    bool ContainsWidget(QLabel *label);

private slots:
    void on_selectDirBtn_clicked();
    void on_clsBtn_clicked();
    void on_startBtn_clicked();
    void OnFormatChangedSlot(const QString &arg1);
};

#endif // WGARCHIVEMAKER_H
