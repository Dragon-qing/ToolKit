/*!
 * @brief 图表基础控件
 * @file basetable.cpp
 * @date 2025/05/17
 * @author Dragon_qing
 */
#include <QHeaderView>

#include "logger.h"
#include "common.h"

#include "basetable.h"
#include "ui_basetable.h"

BaseTable::BaseTable(QWidget *parent)
    : QTableView(parent)
    , ui(new Ui::BaseTable)
{
    ui->setupUi(this);

    m_pModel = new TKTableModel(this);
    this->horizontalHeader()->setSectionResizeMode(QHeaderView::Fixed); // 设置表头不可拖动
    this->verticalHeader()->setVisible(false); // 隐藏行号列
    setAlternatingRowColors(true);
}

BaseTable::~BaseTable()
{
    delete ui;
}

void BaseTable::Refresh()
{
    Bit32 rowCount = m_pModel->rowCount();
    Bit32 colCount = m_pModel->columnCount();

    for (Bit32 row = 0; row < rowCount; row++)
    {
        for (Bit32 col = 0; col < colCount; col++)
        {
            QVariant value = ContentGet(row, col);
            QModelIndex index = m_pModel->index(row, col);
            m_pModel->setData(index, value);
        }
    }
}

void BaseTable::ReDraw()
{
    m_pModel->ClearData();
    Bit32 colCount = m_pModel->columnCount();

    for (Bit32 row = 0; row < TotalRow(); row++)
    {
        QStringList list;
        list.clear();
        for (Bit32 col = 0; col < colCount; col++)
        {
            list << ContentGet(row, col).toString();
        }
        m_pModel->InsertData(list);
    }
}

Bit32 BaseTable::InitTable(QStringList headList, QVector<Bit32> scale)
{
    if (headList.count() != scale.count())
    {
        Logger::Instance().AddLog(CRITICAL_LOG, TR("表格初始化失败, 数据长度不一致"));
        return -1;
    }

    Bit32 scaleSum = 0;
    for (Bit32 i = 0; i < headList.count(); i++)
    {
        QString head = headList.at(i);
        scaleSum += scale.at(i);
        m_scaleVec.append(scale.at(i));
        m_pModel->InsertHead(head);
    }

    for (Bit32 row = 0; row < TotalRow(); row++)
    {
        QStringList list;
        list.clear();
        for (Bit32 col = 0; col < scale.count(); col++)
        {
            list << ContentGet(row, col).toString();
        }
        m_pModel->InsertData(list);
    }

    m_scaleVec.prepend(scaleSum);

    setModel(m_pModel);

    return 0;
}

void BaseTable::resizeEvent(QResizeEvent *event)
{
    QTableView::resizeEvent(event);

    Bit32 totalWidth = viewport()->width();
    Bit32 scaleSum = m_scaleVec.value(0);
    for (Bit32 i = 1; i < m_scaleVec.count(); i++)
    {
        Bit32 colWidth = totalWidth * (fBit64(m_scaleVec.at(i)) / scaleSum);
        setColumnWidth(i - 1, colWidth);
    }
}

TKTableModel::TKTableModel(QWidget *parent)
    : QAbstractTableModel(parent)
{
    m_dataVec.clear();
    m_headList.clear();
}

TKTableModel::~TKTableModel()
{

}

int TKTableModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);

    return m_dataVec.count();
}

int TKTableModel::columnCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);

    return m_headList.count();
}

QVariant TKTableModel::data(const QModelIndex &index, int role) const
{
    if (role != Qt::DisplayRole || !index.isValid())
    {
        return QVariant();
    }

    return m_dataVec.value(index.row()).value(index.column());
}

QVariant TKTableModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (role != Qt::DisplayRole)
        return QVariant();

    if (orientation == Qt::Horizontal && section < m_headList.size()) {
        return m_headList.at(section);
    }

    return QVariant();
}

bool TKTableModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if (!index.isValid() || role != Qt::EditRole)
    {
        return false;
    }

    m_dataVec[index.row()][index.column()] = value.toString();
    return true;
}

void TKTableModel::InsertData(QStringList list)
{
    beginInsertRows(QModelIndex(), m_dataVec.count(), m_dataVec.count());
    m_dataVec.append(list);
    endInsertRows();
}

void TKTableModel::InsertHead(QString head)
{
    m_headList.append(head);
}

void TKTableModel::ClearData()
{
    beginResetModel();
    m_dataVec.clear();
    endResetModel();
}

bool TKTableModel::removeRows(int row, int count, const QModelIndex &parent)
{
    if (row < 0 || row + count > rowCount())
    {
        return false;
    }
    beginRemoveRows(parent, row, row + count - 1);

    // 删除数据
    for (Bit32 i = 0; i < count; ++i) {
        m_dataVec.removeAt(row);
    }

    endRemoveRows();
    return true;
}
