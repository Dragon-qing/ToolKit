/*!
 * @brief 辅助线设置弹窗
 * @file dlgunionplotsubline.cpp
 * @date 2025/05/14
 * @author Dragon_qing
 */

#include <QKeyEvent>

#include "common.h"

#include "dlgunionplotsubline.h"
#include "ui_dlgunionplotsubline.h"

DlgUnionPlotSubline::DlgUnionPlotSubline(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::DlgUnionPlotSubline)
{
    ui->setupUi(this);
    this->setWindowTitle(QObject::TR("添加辅助线"));
    this->setWindowFlags(windowFlags() & ~Qt::WindowContextHelpButtonHint);
    m_enableList << ui->checkBox_1 << ui->checkBox_2 << ui->checkBox_3 << ui->checkBox_4;
    m_stList << ui->lineEditST_1 << ui->lineEditST_2 << ui->lineEditST_3 << ui->lineEditST_4;
    m_edList << ui->lineEditED_1 << ui->lineEditED_2 << ui->lineEditED_3 << ui->lineEditED_4;
}

DlgUnionPlotSubline::~DlgUnionPlotSubline()
{
    delete ui;
}

QList<QPair<fBit64, fBit64>> DlgUnionPlotSubline::ExecAndRet()
{
    QDialog::exec();
    QList<QPair<fBit64, fBit64>> list;

    for (Bit32 i = 0; i < m_enableList.count(); i++)
    {
        if (m_enableList.at(i)->isChecked())
        {
            QPair<fBit64, fBit64> range;
            range.first = m_stList.at(i)->text().toDouble();
            range.second = m_edList.at(i)->text().toDouble();
            list << range;
        }
    }

    return list;
}

void DlgUnionPlotSubline::keyPressEvent(QKeyEvent *event)
{
    if (event->key() == Qt::Key_Escape)
    {
        this->close();
    }

    return QDialog::keyPressEvent(event);
}
