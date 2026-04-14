/*!
 * @brief xml编辑器页面
 * @file wgxmleditor.cpp
 * @author Dragon_qing
 * @date 2025/04/17
 */
#include <QFileDialog>
#include <QStandardPaths>

#include "tklogger.h"

#include "wgxmleditor.h"
#include "ui_wgxmleditor.h"


WgXmlEditor::WgXmlEditor(QWidget *parent)
    : BaseWidget(parent)
    , ui(new Ui::WgXmlEditor)
{
    ui->setupUi(this);

    InitModel();
    // 配置view
    ui->treeView->setModel(m_pTreeModel);
    ui->treeView->setHeaderHidden(true);
    ui->treeView->setEditTriggers(QAbstractItemView::NoEditTriggers);

    ui->listView->setModel(m_pListModel);

    connect(ui->treeView, &QTreeView::clicked, this, &WgXmlEditor::HandleTreeViewClicked);
    connect(m_pListModel, &QStringListModel::dataChanged, this, &WgXmlEditor::HandleListDataChange);

    m_pDlg = new DlgPrompt(DlgPrompt::OK_BUTTON, this);
}

WgXmlEditor::~WgXmlEditor()
{
    delete ui;
}

void WgXmlEditor::InitModel()
{
    // 初始化model
    m_pTreeModel = new QStandardItemModel(this);
    m_pListModel = new QStringListModel(this);
}

void WgXmlEditor::on_pushButton_clicked()
{
    m_pTreeModel->removeRows(0, m_pTreeModel->rowCount());
    m_pListModel->removeRows(0, m_pListModel->rowCount());
}

void WgXmlEditor::on_selectBtn_clicked()
{
    QString desktopPath = QStandardPaths::writableLocation(QStandardPaths::DesktopLocation);
    QString filePath = QFileDialog::getOpenFileName(this, QObject::TR("选择文件"), desktopPath, QObject::TR("XML文件(*.xml)"));
    if (!filePath.isEmpty())
    {
        m_pTreeModel->removeRows(0, m_pTreeModel->rowCount());
    }
    DataXmlEditor::Instance().SetXmlFile(filePath, m_pTreeModel);
    TKLogger::Instance().AddLog(INFO_LOG, TR("xml编辑器-打开%1文件").arg(filePath));
    ui->treeView->expand(m_pTreeModel->index(0, 0));
}

void WgXmlEditor::HandleTreeViewClicked(const QModelIndex &index)
{
    QStringList attr = DataXmlEditor::Instance().GetAttrList(m_pTreeModel, index);
    m_pListModel->setStringList(attr);
    m_currentIndex = index;
}

void WgXmlEditor::HandleListDataChange(const QModelIndex &topLeft, const QModelIndex &bottomRight)
{
    Q_UNUSED(topLeft);
    Q_UNUSED(bottomRight);

    DataXmlEditor::Instance().SyncAttrList(m_pTreeModel, m_currentIndex, m_pListModel->stringList());
}


void WgXmlEditor::on_saveBtn_clicked()
{
    DataXmlEditor::Instance().Save();
    m_pDlg->ExecAndRet(TR("保存完成"));
}

