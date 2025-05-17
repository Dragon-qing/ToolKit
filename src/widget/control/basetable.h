#ifndef BASETABLE_H
#define BASETABLE_H

#include <QWidget>
#include <QTableView>

#include "datadef.h"

namespace Ui {
class BaseTable;
}

class TableModel;
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
    virtual QVariant RoleStyle(Bit32 row, Bit32 col, RoleType role) = 0;

private:
    Ui::BaseTable *ui;
    TableModel *m_pModel;
};

class TableModel : public QAbstractTableModel
{
    Q_OBJECT
public:
    explicit TableModel(QWidget *parent = nullptr);
    ~TableModel();



    // QAbstractItemModel interface
public:
    int rowCount(const QModelIndex &parent) const override;
    int columnCount(const QModelIndex &parent) const override;
    QVariant data(const QModelIndex &index, int role) const override;
}

#endif // BASETABLE_H
