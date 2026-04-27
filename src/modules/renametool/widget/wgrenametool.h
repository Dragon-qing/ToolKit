#ifndef WGRENAMETOOL_H
#define WGRENAMETOOL_H

#include <QSet>

#include "basewidget.h"
#include "basetable.h"
#include "dlgprompt.h"
#include "renametoolservice.h"
#include "renametooldata.h"

namespace Ui {
class WgRenameTool;
}

class PreviewTable;
class OriginFilesTable;

class WgRenameTool : public BaseWidget
{
    Q_OBJECT

public:
    enum class RulesPage{
        COMMON = 0,     // 常用
        SEARCH_REPLACE, // 查找替换

        PAGE_NUM // 页数
    };
    explicit WgRenameTool(QWidget *parent = nullptr);
    ~WgRenameTool();

protected:
    void MessageFlows(QVariant messageid, QVariant messageValue) override;
    QStringList GetHelpText() override;

    void dragEnterEvent(QDragEnterEvent *event) override;
    void dropEvent(QDropEvent *event) override;
private:
    Ui::WgRenameTool *ui;
    RenameToolService m_renameService; // 工具服务实例
    PreviewTable* m_previewTable; // 预览表格
    OriginFilesTable* m_originFilesTable; // 原始文件表格
    QSet<QString> m_selectedFileSet; // 选中文件路径集合
    DlgPrompt* m_pPrompt; // 提示对话框

    void InitRenderWidget(); // 初始化渲染组件
    void UpdateSelectedCount(); // 更新选中文件计数

private slots:
    void on_pushButton_start_clicked();
    void on_pushButton_reset_clicked();
    void on_pushButton_addfile_clicked();
    void on_pushButton_adddir_clicked();
    void on_pushButton_clear_clicked();

    void OnPreviewReadySlot(const QVector<RenamePreviewDTO> &previewList);
    void OnRenameFinishedSlot(const QVector<RenameResultDTO> &resultList);
    void OnErrorSlot(const QString &errorMessage);

};

class PreviewTable : public BaseTable
{
    Q_OBJECT
public:
    enum class Column {
        ORIGINALNAME = 0, // 原文件名
        ARROW,            // "->"符号
        NEWNAME           // 新文件名
    };
    explicit PreviewTable(QWidget *parent = nullptr);
    ~PreviewTable();

    QVariant ContentGet(Bit32 row, Bit32 col) override;
    Bit32 TotalRow() override;
    void SetPreviewList(const QVector<RenamePreviewDTO> &previewList);
    bool IsPreviewValid(Bit32 row) const;
private:
    QVector<RenamePreviewDTO> m_previewList; // 预览数据列表
};

class PreviewTabDelegate : public QStyledItemDelegate
{
public:
    explicit PreviewTabDelegate(QObject *parent = nullptr) : QStyledItemDelegate(parent) {}
    ~PreviewTabDelegate() override = default;

    void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const override;
};

class OriginFilesTable : public BaseTable
{
    Q_OBJECT
public:
    enum class Column {
        FILENAME = 0, // 文件名
        FILEPATH,     // 文件路径
    };
    explicit OriginFilesTable(QWidget *parent = nullptr);
    ~OriginFilesTable();

    QVariant ContentGet(Bit32 row, Bit32 col) override;
    Bit32 TotalRow() override;

    // Getter和Setter
    void AddOriginFiles(const QVector<FileItemDTO> &files);
    void ClearOriginFiles() { m_originFiles.clear(); }
    QVector<FileItemDTO> GetOriginFiles() const { return m_originFiles; }

private:
    QVector<FileItemDTO> m_originFiles; // 原始文件列表

private slots:
    void OnRowMovedSlot(int from, int to); // 行移动槽函数
};
#endif // WGRENAMETOOL_H
