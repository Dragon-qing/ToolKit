#ifndef WGLOG_H
#define WGLOG_H

#include <QTableView>
#include <QStandardItemModel>

#include "basewidget.h"
#include "common.h"
#include "dlgprompt.h"

namespace Ui {
class WgLog;
}

class WgLog : public BaseWidget
{
    Q_OBJECT

public:
    explicit WgLog(QWidget *parent = nullptr);
    ~WgLog();
    void MessageFlows(QVariant messageid, QVariant messageValue) override;

protected:
    void keyPressEvent(QKeyEvent *event) override;

private:
    Ui::WgLog *ui;
    QStandardItemModel *m_pModel; // 日志数据模型
    DlgPrompt *m_pDelDlg; // 删除交互对话框
    DlgPrompt *m_pDetailDlg; // 详情对话框

    QStringList GetHelpText() override;
private slots:
    void DoubleClickHandle(const QModelIndex &index);
};

class LogTab : public QTableView
{
public:
    explicit LogTab(QWidget *parent = nullptr);
    ~LogTab();
    void resizeEvent(QResizeEvent *event);
};

#endif // WGLOG_H
