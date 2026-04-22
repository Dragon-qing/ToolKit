#ifndef BASETABLE_H
#define BASETABLE_H

#include <qobjectdefs.h>
#include <QWidget>
#include <QTableView>
#include <QStyledItemDelegate>

#include "datadef.h"

namespace Ui {
class BaseTable;
}

class TKTableModel;
class BaseTable : public QTableView
{
    Q_OBJECT

public:
    enum RoleType{
        EnableRole = 0,
        EditRole,
        DisEditRole,
        SelectRole,

        RoleNum
    };
    explicit BaseTable(QWidget *parent = nullptr);
    ~BaseTable();

    virtual QVariant ContentGet(Bit32 row, Bit32 col) = 0;
    virtual Bit32 TotalRow() = 0;
    // virtual QVariant RoleStyle(Bit32 row, Bit32 col, RoleType role) = 0;

    void Refresh(); // 仅更新已有数据
    void ReDraw(); // 重新构建数据

protected:
    TKTableModel *m_pModel;
    /**
     * @brief: 初始化表格
     * @param {QStringList} headList: 表头列表
     * @param {QVector<Bit32>} scale: 列宽比例
     * @return {Bit32} 0:成功，其他:失败
     */
    Bit32 InitTable(QStringList headList, QVector<Bit32> scale);

    void resizeEvent(QResizeEvent *event) override;
private:
    Ui::BaseTable *ui;
    QVector<Bit32> m_scaleVec;
};

class TKTableModel : public QAbstractTableModel
{
    Q_OBJECT
public:
    explicit TKTableModel(QWidget *parent = nullptr);
    ~TKTableModel();

    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    int columnCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role) const override;
    QVariant headerData(int section, Qt::Orientation orientation, int role) const override;
    bool setData(const QModelIndex &index, const QVariant &value, int role = Qt::EditRole) override;

    void InsertData(QStringList list); // 尾插数据
    void InsertHead(QString head); // 尾插标题头
    void ClearData(); // 清空数据

    bool removeRows(int row, int count, const QModelIndex &parent) override;
    Qt::ItemFlags flags(const QModelIndex &index) const override;
    QStringList mimeTypes() const override;
    bool dropMimeData(const QMimeData *data, Qt::DropAction action, int row, int column, const QModelIndex &parent) override;
    Qt::DropActions supportedDropActions() const override;
    QMimeData *mimeData(const QModelIndexList &indexes) const override;

signals:
    // 行移动信号
    void RowMoved(int from, int to); 

private:
    QVector<QStringList> m_dataVec; // 数据
    QStringList m_headList; // 标题头
};

class TooltipDelegate : public QStyledItemDelegate {
public:
    explicit TooltipDelegate(QObject *parent = nullptr) : QStyledItemDelegate(parent) {}
    ~TooltipDelegate() override = default;
    bool helpEvent(QHelpEvent *event, QAbstractItemView *view,
                   const QStyleOptionViewItem &option, const QModelIndex &index) override;
};

#endif // BASETABLE_H
