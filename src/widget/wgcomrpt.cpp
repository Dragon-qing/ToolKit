/*!
 * @brief 调机数据分析页面
 * @file wgcomrpt.cpp
 * @author Dragon_qing
 * @date 2025/05/06
 */
#include <math.h>

#include <QFileDialog>
#include <QStringList>
#include <QKeyEvent>
#include <QRegularExpression>

#include "datacomrpt.h"

#include "wgcomrpt.h"
#include "ui_wgcomrpt.h"

WgComRpt::WgComRpt(QWidget *parent)
    : BaseWidget(parent)
    , ui(new Ui::WgComRpt)
{
    ui->setupUi(this);
    m_pDlg = new DlgPrompt(DlgPrompt::OK_BUTTON, this);
    ui->plot->setInteraction(QCP::iRangeDrag, true);
    // ui->plot->setInteraction(QCP::iRangeZoom, true);
    m_xAxisRange = QPair<fBit64, fBit64>(0, 10);
    m_yAxisRange = QPair<fBit64, fBit64>(0, 10);
    InitDict();
    ui->label->setText("");
    connect(ui->defaultRaido, &QRadioButton::toggled, this, &WgComRpt::DefaultRadioHandle);
    connect(ui->progressBar, &QProgressBar::valueChanged, this, &WgComRpt::ProcessChangeHandle);

    m_pTracer = new QCPItemTracer(ui->plot);
    m_pTracer->setStyle(QCPItemTracer::tsCircle);
    m_pTracer->setPen(QPen(Qt::red));
    m_pTracer->setBrush(Qt::red);
    m_pTracer->setSize(7);
    m_pTracer->setVisible(false);
    m_pQCPText = new QCPItemText(ui->plot);
    m_pQCPText->setColor(QColor(Qt::red));
    m_pQCPText->setFont(QFont(FONT_STYLE, 9));
    m_pQCPText->setLayer("overlay");
    m_pQCPText->setVisible(false);

    // 预设组合图初始化
    m_pCurPlot = NULL;
    m_pJourPlot = NULL;
    m_pPosPlot = NULL;
    m_pSpdlDirPlot = NULL;
    m_pSpdlFluPlot = NULL;
    m_pSpeedPlot = NULL;
    m_pSpdlSpeedPlot = NULL;
    m_pToolChangePlot = NULL;
    m_pTapPlot = NULL;
    m_pCirclePlot = NULL;
    m_sOpenFolder = "";

    m_pDlgComRes = new DlgComRes(this);
    ui->plot->installEventFilter(this);
}

WgComRpt::~WgComRpt()
{
    delete ui;
}

void WgComRpt::keyPressEvent(QKeyEvent *event)
{
    if (event->modifiers() & Qt::ControlModifier)
    {
        if (event->key() == Qt::Key_Z)
        {
            ui->plot->xAxis->setRange(m_xAxisRange.first, m_xAxisRange.second);
            ui->plot->yAxis->setRange(m_yAxisRange.first, m_yAxisRange.second);
            ui->plot->replot();
            TKLogger::Instance().AddLog(INFO_LOG, TR("图像复位"));
        }
        else if (event->key() == Qt::Key_R)
        {
            m_pDlgComRes->Exec();
        }
    }
}

bool WgComRpt::eventFilter(QObject *watched, QEvent *event)
{
    if (event->type() == QEvent::Wheel)
    {
        QWheelEvent *wheelEvent = static_cast<QWheelEvent*>(event);
        if (wheelEvent->modifiers() & Qt::ControlModifier)
        {
            if (wheelEvent->angleDelta().y() > 0)
            {
                EnlargeGraph(1);
            }
            else
            {
                ReduceGraph(1);
            }
        }
        else if (wheelEvent->modifiers() & Qt::ShiftModifier)
        {
            if (wheelEvent->angleDelta().y() > 0)
            {
                EnlargeGraph(0);
            }
            else
            {
                ReduceGraph(0);
            }
        }
        else
        {
            if (wheelEvent->angleDelta().y() > 0)
            {
                EnlargeGraph(0, 0.8);
                EnlargeGraph(1, 0.8);
            }
            else
            {
                ReduceGraph(0, 1.2);
                ReduceGraph(1, 1.2);
            }
        }
        wheelEvent->accept();
    }

    return BaseWidget::eventFilter(watched, event);
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
        {"ACT_TRQ", TR("负载电流")},
        {"LINE_NUM", TR("行号")},
        {"F_AXIS_REG", TR("定向信号点")},
        {"MC_0", TR("位置环")},
        {"MC_1", TR("速度环")},
        {"MC_2", TR("圆度测试")},
        {"MC_3", TR("刚性攻丝（同步误差）")},
        {"MC_6", TR("主轴升降速")},
        {"MC_9", TR("换刀时间")},
        {"MC_16", TR("全程电流")},
        {"MC_20", TR("惯量辨识")},
        {"MC_23", TR("主轴波动")},
        {"MC_24", TR("主轴定向")},
    });
}

void WgComRpt::BuildUnioPlot(QString mask)
{
    if (mask == "MC_0") // 位置环
    {
        BuildPosDefaultPlot();
    }
    else if (mask == "MC_1") // 速度环
    {
        BuildSpeedDefaultPlot();
    }
    else if (mask == "MC_2") // 圆度测试
    {
        BuildCircleDefault();
    }
    else if (mask == "MC_3") // 刚性攻丝
    {
        BuildTapDefault();
    }
    else if (mask == "MC_6") // 主轴升降速
    {
        BuildSpdlSpeedDefultPlot();
    }
    else if (mask == "MC_9") // 换刀时间
    {
        BuildToolChangeDefaultPlot();
    }
    else if (mask == "MC_16") // 全程电流
    {
        BuildJourDefaultPlot();
    }
    else if (mask == "MC_23") // 主轴波动
    {
        BuildSpdlFluDefaultPlot();
    }
    else if (mask == "MC_24") // 主轴定向
    {
        BuildSpdlDirDefaultPlot();
    }
    else
    {
        m_pDlg->ExecAndRet(TR("当前文件不支持预设"));
    }

}

void WgComRpt::BuildJourDefaultPlot()
{
    if (m_pJourPlot == NULL)
    {
        m_pJourPlot = new UnionPlot(UnionPlot::VERTICAL_LAYOUT, 4, this);
        m_pJourPlot->SetAxisLabel(QStringList() << TR("位置(mm)") << TR("位置(mm)") << TR("位置(mm)") << TR("位置(mm)"),
                              QStringList() << TR("正向负载电流(mm/min)") << TR("负向负载电流(mm/min)") << TR("正向进给速度(mm/min)") << TR("负向进给速度(mm/min)"));
    }
    QList<QVector<fBit64>> xdata;
    QList<QVector<fBit64>> ydata;
    QVector<fBit64> posX;
    QVector<fBit64> negX;
    QVector<fBit64> posJour;
    QVector<fBit64> negJour;
    QVector<fBit64> posV;
    QVector<fBit64> negV;
    Bit32 totalNum = DataComRpt::Instance().GetTotalPosNum();
    QStringList coefList = DataComRpt::Instance().GetValue(CONFIG_PART, "COEF").toString().split(";");
    fBit64 positionCoef = coefList.at(0).toDouble();
    fBit64 jourCoef = coefList.at(1).toDouble();
    fBit64 actVelCoef = coefList.at(2).toDouble();
    fBit64 cmdVelCoef = coefList.at(4).toDouble();
    fBit64 perVel = 1;
    bool reverseFlag = false;   // 反向标志
    bool beginFlag = true;  // 开始段标志
    Bit32 ignoreCount = totalNum * 0.05; // 忽略前5%的数据开始寻找反向点。
    for (Bit32 i = 0; i < totalNum; i++)
    {
        fBit64 position = DataComRpt::Instance().GetValue(DATA_PART, 0, i).toLongLong() * positionCoef;
        fBit64 jour = DataComRpt::Instance().GetValue(DATA_PART, 1, i).toLongLong() * jourCoef;
        fBit64 actVel = DataComRpt::Instance().GetValue(DATA_PART, 2, i).toLongLong() * actVelCoef;
        fBit64 cmdVel = DataComRpt::Instance().GetValue(DATA_PART, 4, i).toLongLong() * cmdVelCoef;
        Q_UNUSED(cmdVel);

        if (!beginFlag && perVel * actVel < 0)
        {
            // 当前开始反向运动
            reverseFlag = true;
        }
        if (!reverseFlag)
        {
            posX << position;
            posJour << jour;
            posV << actVel;
        }
        else
        {
            negX << position;
            negJour << jour;
            negV << actVel;
        }

        if (actVel != 0.0 && beginFlag && (i > ignoreCount))
        {
            // 当前非起始段，更新标志位
            beginFlag = false;
        }

        if (!beginFlag && actVel != 0.0)
        {
            perVel = actVel;
        }
    }


    xdata << posX << negX << posX << negX;
    ydata << posJour << negJour << posV << negV;
    m_pJourPlot->SetData(&xdata, &ydata, QVector<Bit32>() << 1 << 1 << 1 << 1);
    AddUnionPlot(m_pJourPlot);
}

void WgComRpt::BuildPosDefaultPlot()
{
    if (m_pPosPlot == NULL)
    {
        m_pPosPlot = new UnionPlot(UnionPlot::VERTICAL_LAYOUT, 2, this);
        m_pPosPlot->SetAxisLabel(QStringList() << TR("时间(ms)") << TR("时间(ms)"),
                              QStringList() << TR("指令/实际位置(mm)") << TR("跟踪误差(um)"));
    }
    QList<QVector<fBit64>> xdata;
    QList<QVector<fBit64>> ydata;
    Bit32 totalCount = DataComRpt::Instance().GetTotalPosNum();
    QStringList coefList = DataComRpt::Instance().GetValue(CONFIG_PART, "COEF").toString().split(";");
    QVector<fBit64> followErr;
    fBit64 followErrCoef = coefList.at(0).toDouble();
    QVector<fBit64> actPos;
    fBit64 actPosCoef = coefList.at(1).toDouble();
    QVector<fBit64> cmdPos;
    fBit64 cmdPosCoef = coefList.at(2).toDouble();
    QVector<fBit64> time;
    fBit64 samplePeriod = DataComRpt::Instance().GetValue(CONFIG_PART, "CONF_SMPL_PERIOD").toDouble();

    for (Bit32 i = 0; i < totalCount; i++)
    {
        fBit64 followErrVal = DataComRpt::Instance().GetValue(DATA_PART, 0, i).toLongLong() * followErrCoef;
        fBit64 actPosVal = DataComRpt::Instance().GetValue(DATA_PART, 1, i).toLongLong() * actPosCoef;
        fBit64 cmdPosVal = DataComRpt::Instance().GetValue(DATA_PART, 2, i).toLongLong() * cmdPosCoef;

        followErr << followErrVal;
        actPos << actPosVal;
        cmdPos << cmdPosVal;
        time << i * samplePeriod;
    }

    xdata << time << time;
    ydata << actPos << cmdPos << followErr;
    m_pPosPlot->SetData(&xdata, &ydata, QVector<Bit32>() << 2 << 1, QStringList() << TR("实际位置") << TR("指令位置") << TR("跟踪误差"));
    // 添加至布局
    AddUnionPlot(m_pPosPlot);
}

void WgComRpt::BuildSpdlDirDefaultPlot()
{
    if (m_pSpdlDirPlot == NULL)
    {
        m_pSpdlDirPlot = new UnionPlot(UnionPlot::VERTICAL_LAYOUT, 2, this);
        m_pSpdlDirPlot->SetAxisLabel(QStringList() << TR("时间(ms)") << TR("时间(ms)"),
                              QStringList() << TR("主轴定向信号点") << TR("电流(A)"));
    }
    QList<QVector<fBit64>> xdata;
    QList<QVector<fBit64>> ydata;
    Bit32 totalCount = DataComRpt::Instance().GetTotalPosNum();
    QStringList coefList = DataComRpt::Instance().GetValue(CONFIG_PART, "COEF").toString().split(";");
    QVector<fBit64> reg;
    fBit64 regCoef = coefList.at(0).toDouble();
    QVector<fBit64> current;
    fBit64 currentCoef = coefList.at(1).toDouble();
    QVector<fBit64> time;
    fBit64 samplePeriod = DataComRpt::Instance().GetValue(CONFIG_PART, "CONF_SMPL_PERIOD").toDouble();

    for (Bit32 i = 0; i < totalCount; i++)
    {
        fBit64 regVal = DataComRpt::Instance().GetValue(DATA_PART, 0, i).toLongLong() * regCoef;
        fBit64 currentVal = DataComRpt::Instance().GetValue(DATA_PART, 1, i).toLongLong() * currentCoef;

        reg << regVal;
        current << currentVal;
        time << i * samplePeriod;
    }

    xdata << time << time;
    ydata << reg << current;
    m_pSpdlDirPlot->SetData(&xdata, &ydata, QVector<Bit32>() << 1 << 1);
    // 添加至布局
    AddUnionPlot(m_pSpdlDirPlot);
}

void WgComRpt::BuildSpdlFluDefaultPlot()
{
    if (m_pSpdlFluPlot == NULL)
    {
        m_pSpdlFluPlot = new UnionPlot(UnionPlot::VERTICAL_LAYOUT, 2, this);
        m_pSpdlFluPlot->SetAxisLabel(QStringList() << TR("时间(ms)") << TR("时间(ms)"),
                              QStringList() << TR("主轴转速") << TR("电流(A)"));
    }
    QList<QVector<fBit64>> xdata;
    QList<QVector<fBit64>> ydata;
    Bit32 totalCount = DataComRpt::Instance().GetTotalPosNum();
    QStringList coefList = DataComRpt::Instance().GetValue(CONFIG_PART, "COEF").toString().split(";");
    QVector<fBit64> vel;
    fBit64 velCoef = coefList.at(1).toDouble();
    QVector<fBit64> current;
    fBit64 currentCoef = coefList.at(2).toDouble();
    QVector<fBit64> time;
    fBit64 samplePeriod = DataComRpt::Instance().GetValue(CONFIG_PART, "CONF_SMPL_PERIOD").toDouble();

    for (Bit32 i = 0; i < totalCount; i++)
    {
        fBit64 velVal = DataComRpt::Instance().GetValue(DATA_PART, 1, i).toLongLong() * velCoef;
        fBit64 currentVal = DataComRpt::Instance().GetValue(DATA_PART, 2, i).toLongLong() * currentCoef;

        vel << velVal;
        current << currentVal;
        time << i * samplePeriod;
    }

    xdata << time << time;
    ydata << vel << current;
    m_pSpdlFluPlot->SetData(&xdata, &ydata, QVector<Bit32>() << 1 << 1);
    // 添加至布局
    AddUnionPlot(m_pSpdlFluPlot);
}

void WgComRpt::BuildSpeedDefaultPlot()
{
    if (m_pSpeedPlot == NULL)
    {
        m_pSpeedPlot = new UnionPlot(UnionPlot::VERTICAL_LAYOUT, 2, this);
        m_pSpeedPlot->SetAxisLabel(QStringList() << TR("时间(ms)") << TR("时间(ms)"),
                              QStringList() << TR("指令/实际速度(mm/min)") << TR("加速度(m/s^2)"));
    }
    QList<QVector<fBit64>> xdata;
    QList<QVector<fBit64>> ydata;
    Bit32 totalCount = DataComRpt::Instance().GetTotalPosNum();
    QStringList coefList = DataComRpt::Instance().GetValue(CONFIG_PART, "COEF").toString().split(";");
    QVector<fBit64> cmdVel;
    fBit64 cmdCoef = coefList.at(0).toDouble();
    QVector<fBit64> actVel;
    fBit64 actCoef = coefList.at(1).toDouble();
    QVector<fBit64> acc;
    fBit64 accCoef = 1.0 / 3600000 * 60000; // mm/min^2 -> m/s^2 60000为ms->min
    QVector<fBit64> time;
    fBit64 samplePeriod = DataComRpt::Instance().GetValue(CONFIG_PART, "CONF_SMPL_PERIOD").toDouble();

    for (Bit32 i = 0; i < totalCount; i++)
    {
        fBit64 cmdVal = DataComRpt::Instance().GetValue(DATA_PART, 0, i).toLongLong() * cmdCoef;
        fBit64 actVal = DataComRpt::Instance().GetValue(DATA_PART, 1, i).toLongLong() * actCoef;
        fBit64 nextActVal = 0;
        if (i < totalCount - 1)
        {
            nextActVal = DataComRpt::Instance().GetValue(DATA_PART, 1, i + 1).toLongLong() * actCoef;
        }
        fBit64 accVal = (nextActVal - actVal) / samplePeriod * accCoef;
        cmdVel << cmdVal;
        actVel << actVal;
        time << i * samplePeriod;
        acc << accVal;
    }

    xdata << time << time;
    ydata << cmdVel << actVel << acc;
    m_pSpeedPlot->SetData(&xdata, &ydata, QVector<Bit32>() << 2 << 1, QStringList() << TR("指令速度") << TR("实际速度") << TR("加速度"));
    // 添加至布局
    AddUnionPlot(m_pSpeedPlot);
}

void WgComRpt::BuildSpdlSpeedDefultPlot()
{
    if (m_pSpdlSpeedPlot == NULL)
    {
        m_pSpdlSpeedPlot = new UnionPlot(UnionPlot::VERTICAL_LAYOUT, 2, this);
        m_pSpdlSpeedPlot->SetAxisLabel(QStringList() << TR("时间(ms)") << TR("时间(ms)"),
                              QStringList() << TR("主轴转速(实际/指令)") << TR("负载电流(A)"));
    }
    QList<QVector<fBit64>> xdata;
    QList<QVector<fBit64>> ydata;
    Bit32 totalCount = DataComRpt::Instance().GetTotalPosNum();
    QStringList coefList = DataComRpt::Instance().GetValue(CONFIG_PART, "COEF").toString().split(";");
    QVector<fBit64> actVel;
    fBit64 actCoef = coefList.at(0).toDouble();
    QVector<fBit64> cmdVel;
    fBit64 cmdCoef = coefList.at(3).toDouble();
    QVector<fBit64> current;
    fBit64 currCoef = coefList.at(1).toDouble();
    QVector<fBit64> time;
    fBit64 samplePeriod = DataComRpt::Instance().GetValue(CONFIG_PART, "CONF_SMPL_PERIOD").toDouble();

    for (Bit32 i = 0; i < totalCount; i++)
    {
        fBit64 actVal = DataComRpt::Instance().GetValue(DATA_PART, 0, i).toLongLong() * actCoef;
        fBit64 currVal = DataComRpt::Instance().GetValue(DATA_PART, 1, i).toLongLong() * currCoef;
        fBit64 cmdVal = DataComRpt::Instance().GetValue(DATA_PART, 3, i).toLongLong() * cmdCoef;
        actVel << actVal;
        current << currVal;
        cmdVel << cmdVal;
        time << i * samplePeriod;
    }

    xdata << time << time;
    ydata << actVel << cmdVel << current;
    m_pSpdlSpeedPlot->SetData(&xdata, &ydata, QVector<Bit32>() << 2 << 1, QStringList() << TR("实际速度") << TR("指令速度") << TR("负载电流"));
    // 添加至布局
    AddUnionPlot(m_pSpdlSpeedPlot);
}

void WgComRpt::BuildToolChangeDefaultPlot()
{
    if (m_pToolChangePlot == NULL)
    {
        m_pToolChangePlot = new UnionPlot(UnionPlot::VERTICAL_LAYOUT, 1, this);
        m_pToolChangePlot->SetAxisLabel(QStringList() << TR("时间(ms)"),
                              QStringList() << TR("R%1寄存器").arg(DataComRpt::Instance().GetValue(CONFIG_PART, "CONF_R_REG").toString()));
    }
    QList<QVector<fBit64>> xdata;
    QList<QVector<fBit64>> ydata;
    Bit32 totalCount = DataComRpt::Instance().GetTotalPosNum();
    QStringList coefList = DataComRpt::Instance().GetValue(CONFIG_PART, "COEF").toString().split(";");
    QVector<fBit64> Rreg;
    fBit64 RregCoef = 1;
    QVector<fBit64> time;
    fBit64 samplePeriod = DataComRpt::Instance().GetValue(CONFIG_PART, "CONF_SMPL_PERIOD").toDouble();

    for (Bit32 i = 0; i < totalCount; i++)
    {
        fBit64 reg = DataComRpt::Instance().GetValue(DATA_PART, 0, i).toLongLong() * RregCoef;
        Rreg << reg;
        time << i * samplePeriod;
    }

    xdata << time;
    ydata << Rreg;
    m_pToolChangePlot->SetData(&xdata, &ydata, QVector<Bit32>() << 1);
    // 添加至布局
    AddUnionPlot(m_pToolChangePlot);
}

void WgComRpt::BuildTapDefault()
{
    if (m_pTapPlot == NULL)
    {
        m_pTapPlot = new UnionPlot(UnionPlot::VERTICAL_LAYOUT, 2, this);
        m_pTapPlot->SetAxisLabel(QStringList() << TR("时间(ms)") << TR("时间(ms)"),
                              QStringList() << TR("同步误差(um)") << TR("攻丝轴/同步轴实际速度"));
    }
    QList<QVector<fBit64>> xdata;
    QList<QVector<fBit64>> ydata;
    Bit32 totalCount = DataComRpt::Instance().GetTotalPosNum();
    QStringList coefList = DataComRpt::Instance().GetValue(CONFIG_PART, "COEF").toString().split(";");
    QVector<fBit64> axisVel;
    fBit64 axisVelCoef = coefList.at(0).toDouble() * 60000.0;   // 转换单位
    QVector<fBit64> syncAxisVel;
    fBit64 syncAxisVelCoef = coefList.at(1).toDouble() * 60000.0;
    QVector<fBit64> time;
    fBit64 samplePeriod = 1.0;
    QVector<fBit64> syncErr;
    fBit64 syncErrCoef = 1000;
    fBit64 pitch = DataComRpt::Instance().GetValue(CONFIG_PART, "CONF_PITCH").toDouble();
    fBit64 syncErrVal = 0.0;
    for (Bit32 i = 0; i < totalCount; i++)
    {
        fBit64 axisVal = DataComRpt::Instance().GetValue(DATA_PART, 0, i).toLongLong() * axisVelCoef;
        fBit64 syncAxisVal = DataComRpt::Instance().GetValue(DATA_PART, 1, i).toLongLong() * syncAxisVelCoef;
        syncErrVal += (syncAxisVal * pitch - axisVal) / 60000.0 * samplePeriod * syncErrCoef;
        axisVel << axisVal;
        syncAxisVel << syncAxisVal;
        time << i * samplePeriod;
        syncErr << syncErrVal;
    }

    xdata << time << time;
    ydata << syncErr << axisVel << syncAxisVel;
    m_pTapPlot->SetData(&xdata, &ydata, QVector<Bit32>() << 1 << 2, QStringList() << TR("同步误差") << TR("攻丝轴实际速度") << TR("同步轴实际速度"));
    // 添加至布局
    AddUnionPlot(m_pTapPlot);
}

void WgComRpt::BuildCircleDefault()
{
    if (m_pCirclePlot == NULL)
    {
        QMap<Bit32, QString> axisName({{0, TR("X轴")}, {1, TR("Y轴")}, {2, TR("Z轴")}, {3, TR("A轴")}, {4, TR("C轴")}});
        m_pCirclePlot = new UnionPlot(UnionPlot::VERTICAL_LAYOUT, 1, this);
        m_pCirclePlot->SetAxisLabel(QStringList() << TR("横轴"),
                              QStringList() << TR("纵轴"));
    }
    QList<QVector<fBit64>> xdata;
    QList<QVector<fBit64>> ydata;
    Bit32 totalCount = DataComRpt::Instance().GetTotalPosNum();
    QStringList coefList = DataComRpt::Instance().GetValue(CONFIG_PART, "COEF").toString().split(";");
    QVector<fBit64> hAxis;
    fBit64 hAxisCoef = coefList.at(0).toDouble();
    QVector<fBit64> vAxis;
    fBit64 vAxisCoef = coefList.at(1).toDouble();
    QVector<fBit64> t;

    for (Bit32 i = 0; i < totalCount; i++)
    {
        fBit64 hAxisVal = DataComRpt::Instance().GetValue(DATA_PART, 0, i).toLongLong() * hAxisCoef;
        fBit64 vAxisVal = DataComRpt::Instance().GetValue(DATA_PART, 1, i).toLongLong() * vAxisCoef;

        hAxis << hAxisVal;
        vAxis << vAxisVal;
        t << i;
    }
    xdata << hAxis;
    ydata << vAxis;
    m_pCirclePlot->SetData(&xdata, &ydata, QVector<Bit32>() << 1, QStringList(), true);
    // 添加至布局
    AddUnionPlot(m_pCirclePlot);
}

void WgComRpt::AddUnionPlot(UnionPlot *plot)
{
    if (m_pCurPlot != NULL)
    {
        m_pCurPlot->setVisible(false);
    }
    if (!LayoutContainsWidget(ui->verticalLayout_3, plot))
    {
        ui->verticalLayout_3->addWidget(plot);
    }
    m_pCurPlot = plot;
    plot->setVisible(true);
}

QStringList WgComRpt::GetHelpText()
{
    QStringList list;
    list << QObject::TR("<p align=\"center\"><b>---单图---</b></p>");
    list << QObject::TR("鼠标滚轮：图像放大/缩小");
    list << QObject::TR("Ctrl+鼠标滚轮：纵轴放大/缩小");
    list << QObject::TR("Shift+鼠标滚轮：横轴放大/缩小");
    list << QObject::TR("Ctrl+Z: 复位");
    list << QObject::TR("Ctrl+R: 显示加载的数据计算结果");
    list << QObject::TR("<p align=\"center\"><b>---预设图---</b></p>");
    list << QObject::TR("Ctrl+i: 增加辅助线");
    list << QObject::TR("+/-: 放大缩小纵轴");
    list << QObject::TR("Ctrl+ +/-: 放大缩小横轴轴");
    list << QObject::TR("Ctrl+F: 放大鼠标位置的图像");
    list << QObject::TR("Ctrl+Z: 复位");
    return list;
}

void WgComRpt::EnlargeGraph(Bit16 axis, fBit64 scaleFactor)
{
    QCustomPlot * plot = ui->plot;
    if (axis == 0)
    {
        QCPAxis *xAxis = plot->xAxis;
        QCPRange range = xAxis->range();
        fBit64 center = range.center();
        fBit64 newSize = range.size() * scaleFactor;
        xAxis->setRange(center - newSize / 2, center + newSize / 2);
        plot->replot();
    }
    else if (axis == 1)
    {
        QCPAxis *yAxis = plot->yAxis;
        QCPRange range = yAxis->range();
        fBit64 center = range.center();
        fBit64 newSize = range.size() * scaleFactor;
        yAxis->setRange(center - newSize / 2, center + newSize / 2);
        plot->replot();
    }
}

void WgComRpt::ReduceGraph(Bit16 axis, fBit64 scaleFactor)
{
    QCustomPlot * plot = ui->plot;
    if (axis == 0)
    {
        QCPAxis *xAxis = plot->xAxis;
        QCPRange range = xAxis->range();
        fBit64 center = range.center();
        fBit64 newSize = range.size() * scaleFactor;
        xAxis->setRange(center - newSize / 2, center + newSize / 2);
        plot->replot();
    }
    else if (axis == 1)
    {
        QCPAxis *yAxis = plot->yAxis;
        QCPRange range = yAxis->range();
        fBit64 center = range.center();
        fBit64 newSize = range.size() * scaleFactor;
        yAxis->setRange(center - newSize / 2, center + newSize / 2);
        plot->replot();
    }
}

void WgComRpt::UnionReplot()
{
    if (m_pCurPlot != NULL)
    {
        m_pCurPlot->RePlot();
    }
}

void WgComRpt::on_OpenBtn_clicked()
{
    QString desktopPath = QStandardPaths::writableLocation(QStandardPaths::DesktopLocation);
    QString openPath = "";
    if (m_sOpenFolder.isEmpty())
    {
        openPath = desktopPath;
    }
    QString filePath = QFileDialog::getOpenFileName(this, QObject::TR("选择文件"), openPath);
    m_sOpenFolder = QFileInfo(filePath).absoluteFilePath();
    if (!QFileInfo(filePath).fileName().startsWith("MC_"))
    {
        if (filePath.isEmpty())
        {
            return;
        }
        m_pDlg->ExecAndRet("请选择采样文件！！！！");
        return;
    }

    DataComRpt::Instance().SetPath(filePath);
    TKLogger::Instance().AddLog(INFO_LOG, TR("打开%1采样文件").arg(filePath));
    ReadFileThread *threadp = DataComRpt::Instance().GetThreadClass();
    connect(threadp, &ReadFileThread::ProcessSignal, ui->progressBar, &QProgressBar::setValue);
}

void WgComRpt::ProcessChangeHandle(int changeValue)
{
    if (changeValue == 100)
    {
        // 解绑信号
        disconnect(DataComRpt::Instance().GetThreadClass(), &ReadFileThread::ProcessSignal, ui->progressBar, &QProgressBar::setValue);
        ui->OpenBtn->setEnabled(true);
        QString smplType = DataComRpt::Instance().GetValue(CONFIG_PART, "SMPL_TYPE").toString();
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
        QString fileName = DataComRpt::Instance().GetFileName();
        QRegularExpression re("^(?<type>MC_\\d+)_.*$");
        QRegularExpressionMatch match = re.match(fileName);
        if (match.hasMatch())
        {
            QString labelText = match.captured("type");
            ui->label->setText(m_pDictMap->value(labelText, labelText));
        }
		// 刷新预设图像
        DefaultRadioHandle(ui->defaultRaido->isChecked());
        PromptOut(TR("%1已加载").arg(fileName), 60000);
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
        m_pTracer->setVisible(false);
        m_pQCPText->setVisible(false);
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

        m_pTracer->setGraph(ui->plot->graph(0));
        m_pTracer->setGraphKey(x_posval);
        m_pTracer->setVisible(true);
        m_pQCPText->position->setParentAnchor(m_pTracer->position);
        m_pQCPText->setText(QString("x=%1\ny=%2").arg(x_posval).arg(y_posval));
        m_pQCPText->setTextAlignment(Qt::AlignLeft);
        m_pQCPText->setPositionAlignment(Qt::AlignBottom | Qt::AlignHCenter);  // 文本底部对齐到 tracer
        m_pQCPText->position->setCoords(0, -10);
        m_pQCPText->setVisible(true);

    }
    else
    {
        m_pTracer->setVisible(false);
        m_pQCPText->setVisible(false);
    }
    ui->plot->replot();
}


void WgComRpt::on_drawBtn_clicked()
{
    Bit32 totalNum = DataComRpt::Instance().GetTotalPosNum();
    Bit32 xSamplIdx = ui->xAxisComboBox->currentIndex();
    Bit32 ySamplIdx = ui->yAxisComboBox->currentIndex();
    // 获取转换系数
    QStringList coefStrList = DataComRpt::Instance().GetValue(CONFIG_PART, "COEF").toString().split(";");
    fBit64 xCoef = 1;
    fBit64 yCoef = 1;
    if (xSamplIdx - AXIS_TYPE_NUM >= 0)
    {
        xCoef = coefStrList.at(xSamplIdx - AXIS_TYPE_NUM).toDouble();
    }
    else
    {
        xCoef = (fBit64)DataComRpt::Instance().GetValue(CONFIG_PART, "CONF_SMPL_PERIOD").toInt();
    }
    if (ySamplIdx - AXIS_TYPE_NUM >= 0)
    {
        yCoef = coefStrList.at(ySamplIdx - AXIS_TYPE_NUM).toDouble();
    }
    else
    {
        yCoef = (fBit64)DataComRpt::Instance().GetValue(CONFIG_PART, "CONF_SMPL_PERIOD").toInt();
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
            x = DataComRpt::Instance().GetValue(DATA_PART, xSamplIdx - AXIS_TYPE_NUM, i).toLongLong();
        }
        if (ySamplIdx == TIME)
        {
            y = i;
        }
        else
        {
            y = DataComRpt::Instance().GetValue(DATA_PART, ySamplIdx - AXIS_TYPE_NUM, i).toLongLong();
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
    PromptOut(TR("绘制完成"), 3000);
}

void WgComRpt::DefaultRadioHandle(bool checked)
{
    if (checked) // 使用预定组合图
    {
        QString mask = DataComRpt::Instance().GetMask();
        BuildUnioPlot(mask);
        UnionReplot();
        ui->stackedWidget->setCurrentIndex(1);
    }
    else // 使用自定义图像
    {
        ui->stackedWidget->setCurrentIndex(0);
    }
}

void WgComRpt::MessageFlows(QVariant messageid, QVariant messageValue)
{
    Q_UNUSED(messageValue);
    if (messageid == MsgData::SETFOCUS)
    {
        ui->plot->setFocus();
    }
}

