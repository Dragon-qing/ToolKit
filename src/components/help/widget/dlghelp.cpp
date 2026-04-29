#include <QAbstractItemView>

#include "common.h"
#include "sysapi.h"
#include "helpservice.h"

#include "dlghelp.h"
#include "ui_dlghelp.h"

DlgHelp::DlgHelp(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::DlgHelp)
{
    ui->setupUi(this);
    setWindowFlag(Qt::WindowContextHelpButtonHint, false);
    resize(800, 600);
    setWindowTitle(QObject::TR("帮助"));
    m_pModel = new QStandardItemModel(this);
    ui->treeView->setModel(m_pModel);
    ui->treeView->header()->setFont(QFont(FONT_STYLE, 14, 600));
    ui->treeView->setFont(QFont(FONT_STYLE, 11));
    ui->treeView->setEditTriggers(QAbstractItemView::NoEditTriggers);

    QFont font(FONT_STYLE, 10);
    ui->textBrowser->setFont(font);
    connect(ui->treeView->selectionModel(), &QItemSelectionModel::currentChanged, this, &DlgHelp::OnCategoryChanged);
}

DlgHelp::~DlgHelp()
{
    delete ui;
}

void DlgHelp::Refresh()
{
    ClearModel();
    ui->textBrowser->clear();
    QStandardItem* rootItem = m_pModel->invisibleRootItem();
    QList<QString> categories = HelpService::Instance().GetAllCategories();
    for (auto& category : categories) {
        QStandardItem* categoryItem = new QStandardItem(category);
        rootItem->appendRow(categoryItem);
    }
    ui->treeView->setCurrentIndex(m_pModel->index(0, 0)); // 默认选中第一个类别
    if (categories.isEmpty()) {
        ui->textBrowser->setText(TR("帮助文档暂时为空，等待补充。"));
    }
    ui->treeView->expandAll();
}

void DlgHelp::ClearModel()
{
    m_pModel->clear();
    m_pModel->setHorizontalHeaderLabels({TR("目录")});
}

void DlgHelp::OnCategoryChanged(const QModelIndex &current, const QModelIndex &previous)
{
    Q_UNUSED(previous)
    Bit32 row = current.row();
    if (row < 0 || row >= HelpService::Instance().GetSize()) {
        ui->textBrowser->clear();
        return;
    }

    QStringList contentList = HelpService::Instance().GetHelpContentByIndex(row);
    QString display = "";
    foreach (QString str, contentList)
    {
        display += QString("<p>%1</p>").arg(str);
    }
    if (display.isEmpty())
    {
        display = QObject::TR("<p>帮助文档暂时为空，等待补充。</p>");
    }
    ui->textBrowser->setHtml(display);
}
