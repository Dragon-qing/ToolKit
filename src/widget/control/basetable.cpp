/*!
 * @brief 图表基础控件
 * @file basetable.cpp
 * @date 2025/05/17
 * @author Dragon_qing
 */
#include "basetable.h"
#include "ui_basetable.h"

BaseTable::BaseTable(QWidget *parent)
    : QTableView(parent)
    , ui(new Ui::BaseTable)
{
    ui->setupUi(this);
}

BaseTable::~BaseTable()
{
    delete ui;
}

TableModel::TableModel(QWidget *parent)
    : QAbstractTableModel(parent)
{

}

TableModel::~TableModel()
{

}
