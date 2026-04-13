/*!
 * @brief 日志页面
 * @file wglog.cpp
 * @author Dragon_qing
 * @date 2025/04/27
 */
#include <QKeyEvent>

#include "datalog.h"

#include "wglog.h"
#include "ui_wglog.h"

WgLog::WgLog(QWidget *parent)
    : BaseWidget(parent)
    , ui(new Ui::WgLog)
{
    ui->setupUi(this);

    m_pModel = new QStandardItemModel(this);
    m_pModel->setHorizontalHeaderLabels(QStringList() << "时间" << "类型" << "内容"); // 设置表头
    ui->tableView->setModel(m_pModel);
    m_pDlg = new DlgPrompt(DlgPrompt::OK_BUTTON | DlgPrompt::CANCEL_BUTTON, this);

    DataLog::GetInstance().LoadLog(m_pModel);
    connect(ui->tableView, &LogTab::doubleClicked, this, &WgLog::DoubleClickHandle);
}

WgLog::~WgLog()
{
    delete ui;
}

void WgLog::MessageQueue(QVariant messageid, QVariant messageValue)
{
    Q_UNUSED(messageValue);
    if (messageid == MsgData::REDRAW)
    {
        // 清空内容，重新获取log内容
        m_pModel->removeRows(0, m_pModel->rowCount());
        DataLog::GetInstance().LoadLog(m_pModel);
    }
}

void WgLog::keyPressEvent(QKeyEvent *event)
{
    if (event->modifiers() & Qt::ControlModifier && event->key() == Qt::Key_D)
    {
        Bit32 ret = m_pDlg->ExecAndRet(TR("是否清空日志?"));
        if (ret == DlgPrompt::OK_CODE)
        {
            LogDt::Instance().DeleteAllLog();
            MessageQueue(MsgData::REDRAW, "");
        }
    }
}

QStringList WgLog::GetHelpText()
{
    QStringList list;
    list << TR("<h3>快捷键<h3>");
    list << TR("Ctrl+D 清空日志");

    return list;
}

void WgLog::DoubleClickHandle(const QModelIndex &index)
{
    QString content = m_pModel->data(index, Qt::DisplayRole).toString();
    m_pDlg->ExecAndRet(content);
}

LogTab::LogTab(QWidget *parent)
    : QTableView(parent)
{
    this->horizontalHeader()->setSectionResizeMode(QHeaderView::Fixed); // 设置表头不可拖动
    this->setEditTriggers(QAbstractItemView::NoEditTriggers);  // 设置内容不可编辑
    this->verticalHeader()->setVisible(false); // 隐藏行号列
}

LogTab::~LogTab()
{

}

void LogTab::resizeEvent(QResizeEvent *event)
{
    QTableView::resizeEvent(event);

    int totalWidth = viewport()->width();
    int col1Width = totalWidth * 0.3;
    int col2Width = totalWidth * 0.1;
    int col3Width = totalWidth * 0.6;

    setColumnWidth(0, col1Width);
    setColumnWidth(1, col2Width);
    setColumnWidth(2, col3Width);
}
