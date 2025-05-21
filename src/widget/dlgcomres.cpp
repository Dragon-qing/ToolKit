#include <QMouseEvent>

#include "common.h"
#include "hmicomrpt.h"

#include "dlgcomres.h"
#include "ui_dlgcomres.h"

DlgComRes::DlgComRes(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::DlgComRes)
{
    ui->setupUi(this);

    m_pTab = new ResTable(this);

    ui->verticalLayout->addWidget(m_pTab);
    this->resize(720, 410);

    setWindowFlag(Qt::WindowContextHelpButtonHint, false);
    setWindowTitle(TR("计算结果"));
}

DlgComRes::~DlgComRes()
{
    delete ui;
}

void DlgComRes::Exec()
{
    m_pTab->ReDraw();
    this->show();
}

ResTable::ResTable(QWidget *parent)
    : BaseTable(parent)
{
    QStringList headList;
    headList << TR("ID") << TR("名称") << TR("结果") << TR("范围") << TR("单位");
    QVector<Bit32> scale;
    scale << 4 << 5 << 2 << 4 << 3 ;
    InitTable(headList, scale);
}

ResTable::~ResTable()
{

}

QVariant ResTable::ContentGet(Bit32 row, Bit32 col)
{
    QVariant var = HmiComRpt::Instance().GetValue(RESULT_PART, row);
    ComResult result = var.value<ComResult>();
    if (col == ID_COL)
    {
        return result.resid;
    }
    else if (col == NAME_COL)
    {
        return result.name;
    }
    else if (col == VALUE_COL)
    {
        return result.value;
    }
    else if (col == RANGE_COL)
    {
        return QString("%1~%2").arg(result.range.first).arg(result.range.second);
    }
    else if (col == UNIT_COL)
    {
        return result.unit;
    }

    return "";
}

Bit32 ResTable::TotalRow()
{
    return HmiComRpt::Instance().GetResultCount();
}

bool DlgComRes::eventFilter(QObject *obj, QEvent *event)
{
    if (event->type() == QEvent::MouseButtonPress) {
        QMouseEvent *mouseEvent = static_cast<QMouseEvent *>(event);
        QPoint globalPos = mouseEvent->globalPos();
        if (!this->geometry().contains(globalPos))
        {
            this->reject();  // 点击外部，关闭对话框
            return true;
        }
    }
    return QDialog::eventFilter(obj, event);
}
