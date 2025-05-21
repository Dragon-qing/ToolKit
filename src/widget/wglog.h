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
    void MessageQueue(QVariant messageid, QVariant messageValue);

protected:
    void keyPressEvent(QKeyEvent *event) override;

private:
    Ui::WgLog *ui;
    QStandardItemModel *m_pModel;
    DlgPrompt *m_pDlg;

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
