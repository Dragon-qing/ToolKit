/**
 * @brief 重命名工具界面
 * @file wgrenametool.cpp
 * @date 2026/01/22
 * @author Dragonqing
 */

#include <QTimer>
#include <QDirIterator>
#include <QFileInfo>
#include <QLayoutItem>
#include <QList>
#include <QFileInfo>
#include <QRegularExpression>
#include <QGraphicsDropShadowEffect>
#include <QHeaderView>
#include <QFileDialog>

#include "common.h"
#include "datadef.h"
#include "flowlayout.h"
#include "renametooldata.h"
#include "tklogger.h"

#include "wgrenametool.h"
#include "ui_wgrenametool.h"

WgRenameTool::WgRenameTool(QWidget *parent)
    : BaseWidget(parent)
    , ui(new Ui::WgRenameTool)
{
    ui->setupUi(this);
    m_previewTable = new PreviewTable(this);
    m_originFilesTable = new OriginFilesTable(this);
    ui->widget_preview->layout()->addWidget(m_previewTable);
    ui->widget_originview->layout()->addWidget(m_originFilesTable);
    m_pPrompt = new DlgPrompt(DlgPrompt::OK_BUTTON, this);
    
    InitRenderWidget();
    connect(&m_renameService, &RenameToolService::PreviewReadySignal, this, &WgRenameTool::OnPreviewReadySlot);
    connect(&m_renameService, &RenameToolService::RenameFinishedSignal, this, &WgRenameTool::OnRenameFinishedSlot);
    connect(&m_renameService, &RenameToolService::ErrorSignal, this, &WgRenameTool::OnErrorSlot);
}

WgRenameTool::~WgRenameTool()
{
    delete ui;
}


void WgRenameTool::MessageFlows(QVariant messageid, QVariant messageValue)
{
    Q_UNUSED(messageid)
    Q_UNUSED(messageValue)
}

QStringList WgRenameTool::GetHelpText()
{
    return {};
}

void WgRenameTool::InitRenderWidget()
{
    auto makeShadow = [this]() {
        auto* shadow = new QGraphicsDropShadowEffect(this);
        shadow->setBlurRadius(25);          // 模糊半径（越大越柔和）
        shadow->setOffset(0, 6);            // 偏移（x, y）
        shadow->setColor(QColor(0, 0, 0, 40)); // 颜色（透明黑）
        return shadow;
    };


    ui->frame_select->setGraphicsEffect(makeShadow());
    ui->frame_preview->setGraphicsEffect(makeShadow());
    ui->frame_rules->setGraphicsEffect(makeShadow());
    ui->frame_bottom->setGraphicsEffect(makeShadow());
    ui->pushButton_exportrule->setGraphicsEffect(makeShadow());
    ui->pushButton_importrule->setGraphicsEffect(makeShadow());
    ui->pushButton_reset->setGraphicsEffect(makeShadow());

    QIcon icon(":/img/renametool.svg");
    ui->label_ico->setText("");
    ui->label_ico->setPixmap(icon.pixmap(64, 64));

    ui->tabWidget->setElideMode(Qt::ElideRight);
    ui->tabWidget->setUsesScrollButtons(true);
    
    UpdateSelectedCount();

    m_originFilesTable->setMouseTracking(true);
    m_originFilesTable->viewport()->setMouseTracking(true);
    m_previewTable->setMouseTracking(true);
    m_previewTable->viewport()->setMouseTracking(true);

    // 占位提示
    QFont labelFont(FONT_STYLE, 9);
    FlowLayout* flowLayout = new FlowLayout(ui->widget_placehold);
    ui->widget_placehold->setLayout(flowLayout);
    QStringList tips = {
        TR("{name} 文件名"),
        TR("{ext} 文件扩展名"),
        TR("{date} 文件创建日期"),
        TR("{time} 文件创建时间")
    };
    for (QString& tip : tips) {
        QLabel* label = new QLabel(tip, ui->widget_placehold);
        label->setFont(labelFont);
        flowLayout->addWidget(label);
    }
}

void WgRenameTool::UpdateSelectedCount()
{
    Bit32 totalCount = m_selectedFileSet.size();
    ui->label_filecount->setText(TR("共%1个文件").arg(totalCount));
}

void WgRenameTool::on_pushButton_start_clicked()
{
    RenameTaskDTO task;
    task.files = m_originFilesTable->GetOriginFiles();
    if (task.files.isEmpty()) {
        m_pPrompt->ExecAndRet(TR("请先添加文件"));
        return;
    }

    if (ui->comboBox_fullname->currentIndex() == 0) {
        task.applyRulesToFullName = false;
    } else {
        task.applyRulesToFullName = true;
    }

    task.previewMode = ui->checkBox_preview->isChecked();

    if (ui->tabWidget->currentIndex() == 0) { // 序号规则
        RenameRuleDTO rule1;
        rule1.type = RenameRuleType::Replace;
        rule1.findText = "{name}";
        rule1.replaceText = ui->lineEdit_sequence->text();
        task.rules << rule1;

        RenameRuleDTO rule2;
        rule2.type = RenameRuleType::Sequence;
        rule2.startIndex = ui->spinBox_start->value();
        rule2.step = ui->spinBox_step->value();
        rule2.padding = ui->spinBox_padding->value();
        task.rules << rule2;
    } else if (ui->tabWidget->currentIndex() == 1) { // 查找替换
        // TODO: 支持查找替换
        return;
    } else {
        m_pPrompt->ExecAndRet(TR("请选择一个规则"));
        return;
    }

    m_renameService.SetTask(task);
    m_renameService.Run();
}

void WgRenameTool::on_pushButton_reset_clicked()
{
    UpdateSelectedCount();
}

void WgRenameTool::on_pushButton_addfile_clicked()
{
    QStringList files = QFileDialog::getOpenFileNames(
        this,
        TR("选择多个文件"),
        "",
        "All Files (*)"
    );
    QVector<FileItemDTO> fileItems;
    for (auto& file: files) {
        if (m_selectedFileSet.contains(file)) {
            continue; // 已经添加过的文件跳过
        }
        QFileInfo fileInfo(file);
        FileItemDTO item;
        item.originalPath = file;
        item.originalName = fileInfo.fileName();
        item.baseName = fileInfo.baseName();
        item.extension = fileInfo.suffix();
        fileItems.append(item);
        m_selectedFileSet.insert(file);
    }
    m_originFilesTable->AddOriginFiles(fileItems);
    m_originFilesTable->ReDraw();
    UpdateSelectedCount();
}

void WgRenameTool::on_pushButton_adddir_clicked()
{
    QString dir = QFileDialog::getExistingDirectory(
        this,
        TR("选择目录"),
        "",
        QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks
    );
    if (dir.isEmpty()) {
        return;
    }
    QDir dirObj(dir);
    QVector<FileItemDTO> fileItems;
    for (QFileInfo &fileInfo : dirObj.entryInfoList(QDir::Files | QDir::NoDotAndDotDot)) {
        FileItemDTO item;
        if (m_selectedFileSet.contains(fileInfo.absoluteFilePath())) {
            continue; // 已经添加过的文件跳过
        }

        item.originalPath = fileInfo.absoluteFilePath();
        item.originalName = fileInfo.fileName();
        item.baseName = fileInfo.baseName();
        item.extension = fileInfo.suffix();
        fileItems.append(item);
        m_selectedFileSet.insert(fileInfo.absoluteFilePath());
    }
    m_originFilesTable->AddOriginFiles(fileItems);
    m_originFilesTable->ReDraw();
    UpdateSelectedCount();
}

void WgRenameTool::on_pushButton_clear_clicked()
{
    m_originFilesTable->ClearOriginFiles();
    m_originFilesTable->ReDraw();
    m_previewTable->SetPreviewList({});
    m_previewTable->ReDraw();
    m_selectedFileSet.clear();

    UpdateSelectedCount();
}

void WgRenameTool::OnPreviewReadySlot(const QVector<RenamePreviewDTO> &previewList)
{
    PromptOut(TR("预览完成，条目数: %1").arg(previewList.size()));
    m_previewTable->SetPreviewList(previewList);
}

void WgRenameTool::OnRenameFinishedSlot(const QVector<RenameResultDTO> &resultList)
{
    int successCount = 0;
    for (const RenameResultDTO &item : resultList) {
        if (item.success) {
            ++successCount;
        }
    }
    PromptOut(TR("重命名完成: %1/%2 成功").arg(successCount).arg(resultList.size()));   
}

void WgRenameTool::OnErrorSlot(const QString &errorMessage)
{
    TKLogger::Instance().AddLog(ERROR_LOG, TR("重命名错误: %1").arg(errorMessage));
}

// ---------------------PreviewTable----------------
PreviewTable::PreviewTable(QWidget *parent)
    : BaseTable(parent)
{
    QStringList headList;
    headList << TR("原文件名") << TR("") << TR("新文件名");
    QVector<Bit32> scale = {4, 1, 4};
    InitTable(headList, scale);
    setShowGrid(false); // 隐藏表格线
    horizontalHeader()->setDefaultAlignment(Qt::AlignLeft | Qt::AlignVCenter); // 表头左对齐
    setItemDelegate(new PreviewTabDelegate(this));

}

PreviewTable::~PreviewTable()
{

}

QVariant PreviewTable::ContentGet(Bit32 row, Bit32 col)
{
    if (row < 0 || row >= m_previewList.size())
        return QVariant();

    const RenamePreviewDTO &item = m_previewList.at(row);
    Column column = static_cast<Column>(col);
    switch (column) {
        case Column::ORIGINALNAME: 
            return item.originalName;
        case Column::ARROW:
            return "->";
        case Column::NEWNAME: 
            return item.newName;
        default: 
            return QVariant();
    }

    return QVariant();
}

Bit32 PreviewTable::TotalRow()
{
    return m_previewList.size();
}

void PreviewTable::SetPreviewList(const QVector<RenamePreviewDTO> &previewList)
{
    m_previewList.clear();
    m_previewList = previewList;
    ReDraw();
}

bool PreviewTable::IsPreviewValid(Bit32 row) const
{
    if (row < 0 || row >= m_previewList.size()) {
        return true;
    }
    return m_previewList.at(row).valid;
}

void PreviewTabDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const 
{
    QStyleOptionViewItem opt = option;
    initStyleOption(&opt, index);

    const PreviewTable *table = qobject_cast<const PreviewTable *>(opt.widget);
    if (table && !table->IsPreviewValid(index.row())) {
        opt.palette.setColor(QPalette::Text, QColor("#ff0000"));
    }
    else {
        opt.palette.setColor(QPalette::Text, QColor("#107c10"));
    }

    QStyledItemDelegate::paint(painter, opt, index);
}

// ---------------------OriginFilesTable----------------
OriginFilesTable::OriginFilesTable(QWidget *parent)
    : BaseTable(parent)
{
    QStringList headList;
    headList << TR("原文件名") << TR("路径");
    QVector<Bit32> scale = {5, 2};
    InitTable(headList, scale);
    horizontalHeader()->setDefaultAlignment(Qt::AlignLeft | Qt::AlignVCenter); // 表头左对齐

    setDragEnabled(true);
    setAcceptDrops(true);
    setDropIndicatorShown(true); // 显示拖放指示器

    setDragDropMode(QAbstractItemView::InternalMove); // 启用内部拖放模式
    setDefaultDropAction(Qt::MoveAction); // 默认拖放操作为移动

    // 整行拖动
    setSelectionBehavior(QAbstractItemView::SelectRows);
    setSelectionMode(QAbstractItemView::SingleSelection);

    connect(m_pModel, &TKTableModel::RowMoved, this, &OriginFilesTable::OnRowMovedSlot);
}

OriginFilesTable::~OriginFilesTable()
{
    
}

QVariant OriginFilesTable::ContentGet(Bit32 row, Bit32 col)
{
    if (row < 0 || row >= m_originFiles.size())
        return QVariant();

    const FileItemDTO &item = m_originFiles.at(row);
    Column column = static_cast<Column>(col);
    switch (column) {
        case Column::FILENAME: 
            return item.originalName;
        case Column::FILEPATH:
            return QFileInfo(item.originalPath).path();
        default: 
            return QVariant();
    }

    return QVariant();
}

Bit32 OriginFilesTable::TotalRow()
{
    return m_originFiles.size();
}

void OriginFilesTable::AddOriginFiles(const QVector<FileItemDTO> &files)
{
    m_originFiles << files;
}


void OriginFilesTable::OnRowMovedSlot(int from, int to)
{
    m_originFiles.move(from, to);
}
