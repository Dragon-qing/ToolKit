#ifndef WGLOG_H
#define WGLOG_H

#include <QTableView>
#include <QStandardItemModel>

#include "basewidget.h"
#include "common.h"

namespace Ui {
class WgLog;
}

class WgLog : public BaseWidget
{
    Q_OBJECT

public:
    explicit WgLog(QWidget *parent = nullptr);
    ~WgLog();
    void MassageQueue(QVariant messageid, QVariant messageValue);

private:
    Ui::WgLog *ui;

    QStandardItemModel *m_pModel;

};

class LogTab : public QTableView
{
public:
    explicit LogTab(QWidget *parent = nullptr);
    ~LogTab();
    void resizeEvent(QResizeEvent *event);
};

#endif // WGLOG_H
