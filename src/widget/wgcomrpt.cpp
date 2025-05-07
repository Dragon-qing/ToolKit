/*!
 * @brief 调机数据分析页面
 * @file wgcomrpt.cpp
 * @author Dragon_qing
 * @date 2025/05/06
 */
#include <QFileDialog>
#include <QStringList>
#include <math.h>
#include <QKeyEvent>
#include <QRegularExpression>

#include "hmicomrpt.h"

#include "wgcomrpt.h"
#include "ui_wgcomrpt.h"

WgComRpt::WgComRpt(QWidget *parent)
    : BaseWidget(parent)
    , ui(new Ui::WgComRpt)
{
    ui->setupUi(this);
    m_pDlg = new DlgPrompt(DlgPrompt::OK_BUTTON, this);
    ui->plot->setInteraction(QCP::iRangeDrag, true);
    ui->plot->setInteraction(QCP::iRangeZoom, true);
    m_xAxisRange = QPair<fBit64, fBit64>(0, 10);
    m_yAxisRange = QPair<fBit64, fBit64>(0, 10);
    InitDict();
    ui->label->setText("");

    ReadFileThread *threadp = HmiComRpt::Instance().GetThreadClass();
    connect(threadp, &ReadFileThread::ProcessSignal, ui->progressBar, &QProgressBar::setValue);
    connect(ui->progressBar, &QProgressBar::valueChanged, this, &WgComRpt::ProcessChangeHandle);
}

WgComRpt::~WgComRpt()
{
    delete ui;
}

void WgComRpt::keyPressEvent(QKeyEvent *event)
{
    if (event->modifiers() & Qt::ControlModifier && event->key() == Qt::Key_Z)
    {
        ui->plot->xAxis->setRange(m_xAxisRange.first, m_xAxisRange.second);
        ui->plot->yAxis->setRange(m_yAxisRange.first, m_yAxisRange.second);
        ui->plot->replot();
    }
}

void WgComRpt::InitDict()
{
    m_pDictMap = new QMap<QString, QString>(
    {
        {"CMD_INC", TR("指令速度")},
        {"ACT_VEL", TR("实际速度")},
        {"FOLLOW_ERR", TR("跟踪误差")},
        {"AXIS_ACT", TR("实际位置")},
        {"AXIS_CMD", TR("指令位置")},
        {"MC_0", TR("位置环")},
        {"MC_1", TR("速度环")},
        {"MC_2", TR("圆度测试")},
        {"MC_3", TR("刚性攻丝（同步误差）")},
        {"MC_6", TR("主轴升降速")},
        {"MC_16", TR("全程电流")},
        {"MC_20", TR("惯量辨识")},
        {"MC_23", TR("主轴波动")},
        {"MC_24", TR("主轴定向")},
    });
}

void WgComRpt::on_OpenBtn_clicked()
{
    QString desktopPath = QStandardPaths::writableLocation(QStandardPaths::DesktopLocation);
    QString filePath = QFileDialog::getOpenFileName(this, QObject::TR("选择文件"), desktopPath);
    if (!QFileInfo(filePath).fileName().startsWith("MC_"))
    {
        m_pDlg->ExecAndRet("请选择采样文件！！！！");
        return;
    }

    HmiComRpt::Instance().SetPath(filePath);
}

void WgComRpt::ProcessChangeHandle(int changeValue)
{
    if (changeValue == 100)
    {
        ui->OpenBtn->setEnabled(true);
        QString smplType = HmiComRpt::Instance().GetValue(CONFIG_PART, "SMPL_TYPE").toString();
        if (smplType.endsWith(";"))
        {
            smplType.remove(smplType.size() - 1, 1);
        }
        QStringList smplTypelist = smplType.split(";");
        smplTypelist.insert(0, TR("时间(ms)"));
        for (Bit32 i = 0; i < smplTypelist.count(); i++)
        {
            QString raw = smplTypelist.at(i);
            raw = m_pDictMap->value(raw, raw);
            smplTypelist.replace(i, raw);
        }
        ui->xAxisComboBox->addItems(smplTypelist);
        ui->yAxisComboBox->addItems(smplTypelist);
        ui->yAxisComboBox->setCurrentIndex(AXIS_TYPE_NUM);
        QString fileName = HmiComRpt::Instance().GetFileName();
        QRegularExpression re("^(?<type>MC_\\d+)_.*$");
        QRegularExpressionMatch match = re.match(fileName);
        if (match.hasMatch())
        {
            QString labelText = match.captured("type");
            ui->label->setText(m_pDictMap->value(labelText, labelText));
        }
    }
    else if (changeValue > 0 && ui->OpenBtn->isEnabled())
    {
        ui->label->setText("");
        ui->OpenBtn->setEnabled(false);
        ui->xAxisComboBox->clear();
        ui->yAxisComboBox->clear();
    }
}

void WgComRpt::MouseMoveEventHandle(QMouseEvent *event)
{
    if (ui->plot->graphCount() == 0)
    {
        return;
    }

    //获取鼠标坐标，相对父窗体坐标
    Bit32 x_pos = event->pos().x();
    Bit32 y_pos = event->pos().y();

    //鼠标坐标转化为CustomPlot内部坐标
    fBit32 x_val = ui->plot->xAxis->pixelToCoord(x_pos);
    fBit32 y_val = ui->plot->yAxis->pixelToCoord(y_pos);

    //通过坐标轴范围判断光标是否在点附近
    fBit32 x_begin = ui->plot->xAxis->range().lower;
    fBit32 x_end = ui->plot->xAxis->range().upper;
    fBit32 y_begin = ui->plot->yAxis->range().lower;
    fBit32 y_end = ui->plot->yAxis->range().upper;
    fBit32 x_tolerate = (x_end - x_begin) / 40;//光标与最近点距离在此范围内，便显示该最近点的值
    fBit32 y_tolerate = (y_end - y_begin) / 40;

    //判断有没有超出坐标轴范围
    if (x_val < x_begin || x_val > x_end)
    {
        QToolTip::hideText();
        return;
    }

    //通过x值查找离曲线最近的一个key值索引
    Bit32 index = 0;
    Bit32 index_left = ui->plot->graph(0)->findBegin(x_val, true);//左边最近的一个key值索引
    Bit32 index_right = ui->plot->graph(0)->findEnd(x_val, true);//右边
    fBit32 dif_left = fabs(ui->plot->graph(0)->data()->at(index_left)->key - x_val);
    fBit32 dif_right = fabs(ui->plot->graph(0)->data()->at(index_right)->key - x_val);
    index = ((dif_left < dif_right) ? index_left : index_right);

    double x_posval = ui->plot->graph(0)->data()->at(index)->key;//通过得到的索引获取key值
    double y_posval = ui->plot->graph(0)->data()->at(index)->value;//通过得到的索引获取value值

    fBit32 dx = fabs(x_posval - x_val);
    fBit32 dy = fabs(y_posval - y_val);

    Bit32 graphIndex = 0;//curve index closest to the cursor
    //通过遍历每条曲线在index处的value值，得到离光标点最近的value及对应曲线索引
    for (Bit32 i = 0, n = ui->plot->xAxis->graphs().count(); i < n; i++)
    {
        y_posval = fabs(ui->plot->graph(i)->data()->at(index)->value - y_val);
        if (y_posval < dy)
        {
            dy = y_posval;
            graphIndex = i;
        }
    }

    //判断光标点与最近点的距离是否在设定范围内
    if (dy <= y_tolerate && dx <= x_tolerate)
    {
        y_posval = ui->plot->graph(graphIndex)->data()->at(index)->value;

        QString strToolTip = QString("Graph:%1 \nx=%2\ny=%3").arg(graphIndex + 1).arg(x_posval).arg(y_posval);

        QToolTip::showText(cursor().pos(), strToolTip, ui->plot);
    }
    else
    {
        QToolTip::hideText();
    }
}


void WgComRpt::on_drawBtn_clicked()
{
    Bit32 totalNum = HmiComRpt::Instance().GetTotalPosNum();
    Bit32 xSamplIdx = ui->xAxisComboBox->currentIndex();
    Bit32 ySamplIdx = ui->yAxisComboBox->currentIndex();
    // 获取转换系数
    QStringList coefStrList = HmiComRpt::Instance().GetValue(CONFIG_PART, "COEF").toString().split(";");
    fBit64 xCoef = 1;
    fBit64 yCoef = 1;
    if (xSamplIdx - AXIS_TYPE_NUM >= 0)
    {
        xCoef = coefStrList.at(xSamplIdx - AXIS_TYPE_NUM).toDouble();
    }
    else
    {
        xCoef = (fBit64)HmiComRpt::Instance().GetValue(CONFIG_PART, "CONF_SMPL_PERIOD").toInt();
    }
    if (ySamplIdx - AXIS_TYPE_NUM >= 0)
    {
        yCoef = coefStrList.at(ySamplIdx - AXIS_TYPE_NUM).toDouble();
    }
    else
    {
        yCoef = (fBit64)HmiComRpt::Instance().GetValue(CONFIG_PART, "CONF_SMPL_PERIOD").toInt();
    }
    // 清空画布，增加新画布和轴名
    if (ui->plot->graphCount() != 0)
    {
        disconnect(ui->plot, &QCustomPlot::mouseMove, this, &WgComRpt::MouseMoveEventHandle);
    }
    ui->plot->clearGraphs();
    ui->plot->addGraph();
    ui->plot->xAxis->setLabel(ui->xAxisComboBox->currentText());
    ui->plot->yAxis->setLabel(ui->yAxisComboBox->currentText());

    // 获取点位数据
    QVector<double> xVec;
    QVector<double> yVec;
    fBit64 xmin = 0,xmax = 0,ymin = 0,ymax = 0;
    for (Bit32 i = 0; i < totalNum; i ++)
    {
        Bit64 x = 0;
        Bit64 y = 0;
        if (xSamplIdx == TIME)
        {
            x = i;
        }
        else
        {
            x = HmiComRpt::Instance().GetValue(DATA_PART, xSamplIdx - AXIS_TYPE_NUM, i).toLongLong();
        }
        if (ySamplIdx == TIME)
        {
            y = i;
        }
        else
        {
            y = HmiComRpt::Instance().GetValue(DATA_PART, ySamplIdx - AXIS_TYPE_NUM, i).toLongLong();
        }
        fBit64 xValue = x * xCoef;
        fBit64 yValue = y * yCoef;
        xmin = qMin(xmin, xValue);
        xmax = qMax(xmax, xValue);
        ymin = qMin(ymin, yValue);
        ymax = qMax(ymax, yValue);

        xVec.append(xValue);
        yVec.append(yValue);
    }
    m_xAxisRange = QPair<fBit64, fBit64>(xmin - 20, xmax + 20);
    m_yAxisRange = QPair<fBit64, fBit64>(ymin - 20, ymax + 20);
    ui->plot->xAxis->setRange(xmin - 20, xmax + 20);
    ui->plot->yAxis->setRange(ymin - 20, ymax + 20);
    ui->plot->graph(0)->setData(xVec, yVec);
    ui->plot->replot();
    connect(ui->plot, &QCustomPlot::mouseMove, this, &WgComRpt::MouseMoveEventHandle);
}

