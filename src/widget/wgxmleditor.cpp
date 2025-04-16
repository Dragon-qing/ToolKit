#include "logdt.h"

#include "wgxmleditor.h"
#include "ui_wgxmleditor.h"

WgXmlEditor::WgXmlEditor(QWidget *parent)
    : BaseWidget(parent)
    , ui(new Ui::WgXmlEditor)
{
    ui->setupUi(this);
    InitModel();
    // 配置view
    ui->treeView->setModel(m_pModel);
    ui->treeView->setHeaderHidden(true);
    LogDt::Instance().AddLog(INFO_LOG, "测试");
}

WgXmlEditor::~WgXmlEditor()
{
    delete ui;
}

void WgXmlEditor::InitModel()
{
    m_pModel = new QStandardItemModel(this);
    m_pModel->setHorizontalHeaderLabels(QStringList() << "XML结构");

    // 构建树结构
    QStandardItem *rootItem = m_pModel->invisibleRootItem();

    // 添加一个父节点
    QStandardItem *parentItem = new QStandardItem("父节点1");
    parentItem->appendRow(new QStandardItem("子节点1-1"));
    parentItem->appendRow(new QStandardItem("子节点1-2"));

    // 添加第二个父节点
    QStandardItem *parentItem2 = new QStandardItem("父节点2");
    parentItem2->appendRow(new QStandardItem("子节点2-1"));

    // 插入父节点到根节点
    rootItem->appendRow(parentItem);
    rootItem->appendRow(parentItem2);
}

void WgXmlEditor::on_pushButton_clicked()
{
    m_pModel->removeRows(0, m_pModel->rowCount());
}
