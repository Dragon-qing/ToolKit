#ifndef DLGCOMRES_H
#define DLGCOMRES_H

#include <QDialog>

#include "basetable.h"

QT_BEGIN_NAMESPACE
class ResTable;
QT_END_NAMESPACE

namespace Ui {
class DlgComRes;
}

class DlgComRes : public QDialog
{
    Q_OBJECT

public:

    explicit DlgComRes(QWidget *parent = nullptr);
    ~DlgComRes();
    void Exec();

    bool eventFilter(QObject *obj, QEvent *event) override;

private:
    Ui::DlgComRes *ui;
    ResTable *m_pTab;
};

class ResTable : public BaseTable
{
    Q_OBJECT
public:
    enum _COL_TYPE{
        ID_COL = 0,
        NAME_COL,
        VALUE_COL,
        RANGE_COL,
        UNIT_COL
    };
    explicit ResTable(QWidget *parent = nullptr);
    ~ResTable();

    QVariant ContentGet(Bit32 row, Bit32 col) override;
    Bit32 TotalRow() override;
};
#endif // DLGCOMRES_H
