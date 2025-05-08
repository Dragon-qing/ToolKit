/*!
 * @brief 组合绘图模块
 * @file unionplot.cpp
 * @author Dragon_qing
 * @date 2025/05/08
 */
#include "unionplot.h"
#include "ui_unionplot.h"

/**
 * @brief UnionPlot::UnionPlot 构造函数
 * @param layout 布局种类
 * @param plotNum 绘图数量
 * @param parent 父类指针
 */
UnionPlot::UnionPlot(Layout_Type layout, Bit32 plotNum, QWidget *parent)
    : BaseWidget(parent)
    , ui(new Ui::UnionPlot)
{
    ui->setupUi(this);
    Bit32 columMax = qFloor(qSqrt(plotNum));

    for (Bit32 i = 0; i < plotNum; i++)
    {
        QCustomPlot *plotp = new QCustomPlot(this);
        plotp->setInteraction(QCP::iRangeDrag, true);
        plotp->setInteraction(QCP::iRangeZoom, true);
        if (layout == HORIZON_LAYOUT)   // 水平布局
        {
            ui->gridLayout->addWidget(plotp, 0, i);
        }
        else if (layout == VERTICAL_LAYOUT) // 垂直布局
        {
            ui->gridLayout->addWidget(plotp, i, 0);
        }
        else if (layout == RECTANGLE_LAYOUT)  // 矩形布局
        {
            Bit32 row = i / columMax;
            Bit32 col = i % columMax;
            ui->gridLayout->addWidget(plotp, row, col);
        }
        m_plotList << plotp;
    }

    m_xRangeList.clear();
    m_yRangeList.clear();
}

UnionPlot::~UnionPlot()
{
    delete ui;
    for (Bit32 i = 0; i < m_plotList.count(); i++)
    {
        delete m_plotList.at(i);
    }
    m_plotList.clear();
    m_xRangeList.clear();
    m_yRangeList.clear();
}

/**
 * @brief UnionPlot::SetData 设置数据
 * @param xdata
 * @param ydata
 */
void UnionPlot::SetData(QList<QVector<fBit64> > xdata, QList<QVector<fBit64> > ydata)
{
    m_xRangeList.clear();
    m_yRangeList.clear();
    for (Bit32 i = 0; i < m_plotList.count(); i++)
    {
        QCustomPlot *plot = m_plotList.at(i);
        plot->clearGraphs();
        plot->addGraph();
        plot->graph(0)->setData(xdata.at(i), ydata.at(i));
        m_xRangeList << GetRange(xdata.at(i));
        m_yRangeList << GetRange(ydata.at(i));
    }
}

/**
 * @brief UnionPlot::SetAxisLabel 设置轴标签
 * @param xlabelList
 * @param ylabelList
 */
void UnionPlot::SetAxisLabel(QStringList xlabelList, QStringList ylabelList)
{
    if (xlabelList.count() != ylabelList.count() || xlabelList.count() < m_plotList.count())
    {
        return;
    }

    for (Bit32 i = 0; i < m_plotList.count(); i++)
    {
        QCustomPlot *plot = m_plotList.at(i);
        plot->xAxis->setLabel(xlabelList.at(i));
        plot->yAxis->setLabel(ylabelList.at(i));
    }
}

/**
 * @brief UnionPlot::RePlot 重绘所有图像
 */
void UnionPlot::RePlot()
{
    for (Bit32 i = 0;i < m_plotList.count(); i++)
    {
        QCustomPlot *plot = m_plotList.at(i);
        QPair<fBit64, fBit64> xRange = m_xRangeList.at(i);
        QPair<fBit64, fBit64> yRange = m_yRangeList.at(i);

        plot->xAxis->setRange(xRange.first - 50, xRange.second + 50);
        plot->yAxis->setRange(yRange.first - 50, yRange.second + 50);
        plot->replot();
    }
}
