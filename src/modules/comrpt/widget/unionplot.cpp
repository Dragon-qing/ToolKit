/*!
 * @brief 组合绘图模块
 * @file unionplot.cpp
 * @author Dragon_qing
 * @date 2025/05/08
 */
#include "unionplot.h"
#include "ui_unionplot.h"

const QList<QColor> LINE_COLOR = {QColor(0, 120, 212), QColor(0, 178, 148), QColor(180, 0, 158), QColor(92, 45, 145)};

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
        // plotp->setInteraction(QCP::iRangeZoom, true);
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
        plotp->installEventFilter(this);
        m_plotList << plotp;
        // 初始化跟踪点
        QCPItemTracer *tracer = new QCPItemTracer(plotp);
        tracer->setBrush(Qt::red);
        tracer->setPen(QPen(Qt::red));
        tracer->setStyle(QCPItemTracer::tsCircle);
        tracer->setSize(7);
        tracer->setVisible(false);
        m_pTracer << tracer;
        // 初始化文本信息
        QCPItemText *text = new QCPItemText(plotp);
        text->setColor(QColor(Qt::red));
        text->setFont(QFont(FONT_STYLE, 9));
        text->setLayer("overlay");
        text->setVisible(false);
        m_pText << text;
    }

    m_xRangeList.clear();
    m_yRangeList.clear();
    m_nCurBlock = 0;
    m_nPreBlock = 0;
    m_pDlg = new DlgUnionPlotSubline(this);
    m_vlineList.clear();
    this->setMouseTracking(true);
    installEventFilter(this);
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
    m_vlineList.clear();
}

/**
 * @brief UnionPlot::SetData 设置数据
 * @param xdata
 * @param ydata
 */
void UnionPlot::SetData(QList<QVector<fBit64> > *xdata, QList<QVector<fBit64> > *ydata, QVector<Bit32> graphNum, QStringList lineName, bool isCurve)
{
    m_xRangeList.clear();
    m_yRangeList.clear();
    Bit32 totalGraph = 0;
    for (Bit32 i = 0; i < m_plotList.count(); i++)
    {
        QCustomPlot *plot = m_plotList.at(i);
        Bit32 graphN = graphNum[i];
        QPair<fBit64, fBit64> yrange(ydata->at(i).first(), ydata->at(i).first());
        plot->clearGraphs();
        plot->clearPlottables();
        if (graphN > 1)
        {
            plot->legend->setVisible(true);
        }
        for (Bit32 j = 0 ; j < graphN; j++)
        {
            if (isCurve)
            {
                QCPCurve *curve = new QCPCurve(plot->xAxis, plot->yAxis);
                QVector<fBit64> t;
                for (fBit64 tmp= 0; tmp < xdata->at(i).count(); tmp++)
                {
                    t << tmp;
                }
                curve->setPen(QPen(LINE_COLOR.at(j)));
                curve->setData(t, xdata->at(i), ydata->at(i));
            }
            else
            {
                plot->addGraph();
                plot->graph(j)->setPen(QPen(LINE_COLOR.at(j)));
                plot->graph(j)->setName(lineName.value(totalGraph + j));
                plot->graph(j)->setData(xdata->at(i), ydata->at(totalGraph + j));
            }
            FitRange(yrange, GetRange(ydata->at(totalGraph + j)));
        }
        totalGraph += graphN;
        m_xRangeList << GetRange(xdata->at(i));
        m_yRangeList << yrange;
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
        plot->setVisible(true);
        QPair<fBit64, fBit64> xRange = m_xRangeList.at(i);
        QPair<fBit64, fBit64> yRange = m_yRangeList.at(i);

        fBit64 xOffset = (xRange.second - xRange.first) * 0.1;
        fBit64 yOffset = (yRange.second - yRange.first) * 0.1;
        if (xOffset == 0)
        {
            xOffset += 5;
        }
        if (yOffset == 0)
        {
            yOffset += 5;
        }

        plot->xAxis->setRange(xRange.first - xOffset, xRange.second + xOffset);
        plot->yAxis->setRange(yRange.first - yOffset, yRange.second + yOffset);
        plot->replot();
    }
}

bool UnionPlot::eventFilter(QObject *obj, QEvent *event)
{

    if (event->type() == QEvent::MouseMove)
    {
        Bit32 block = -1;
        for (Bit32 i = 0; i < m_plotList.count(); i++)
        {
            if (obj == m_plotList.at(i))
            {
                block = i;
                break;
            }
        }
        if (block >= 0)
        {
            m_nCurBlock = block;
            QMouseEvent *mouseEvent = static_cast<QMouseEvent *>(event);
            MouseMoveHandleSlot(mouseEvent, block);
        }
        if (m_nPreBlock != m_nCurBlock)
        {
            m_nPreBlock = m_nCurBlock;
            for(Bit32 i = 0; i < m_pTracer.count(); i++)
            {
                if (i != m_nCurBlock)
                {
                    m_pTracer.at(i)->setVisible(false);
                    m_pText.at(i)->setVisible(false);
                    m_plotList.at(i)->replot();
                }
            }
        }
    }
    else if (event->type() == QEvent::Wheel)
    {
        QWheelEvent *wheelEvent = static_cast<QWheelEvent*>(event);
        if (wheelEvent->modifiers() & Qt::ControlModifier)
        {
            if (wheelEvent->angleDelta().y() > 0)
            {
                EnlargeGraph(m_nCurBlock, 1);
            }
            else
            {
                ReduceGraph(m_nCurBlock, 1);
            }
        }
        else if (wheelEvent->modifiers() & Qt::ShiftModifier)
        {
            if (wheelEvent->angleDelta().y() > 0)
            {
                EnlargeGraph(m_nCurBlock, 0);
            }
            else
            {
                ReduceGraph(m_nCurBlock, 0);
            }
        }
        else
        {
            if (wheelEvent->angleDelta().y() > 0)
            {
                EnlargeGraph(m_nCurBlock, 0, 0.8);
                EnlargeGraph(m_nCurBlock, 1, 0.8);
            }
            else
            {
                ReduceGraph(m_nCurBlock, 0, 1.2);
                ReduceGraph(m_nCurBlock, 1, 1.2);
            }
        }
        wheelEvent->accept();
    }

    return BaseWidget::eventFilter(obj, event);
}

void UnionPlot::MouseMoveHandleSlot(QMouseEvent *event, Bit32 block)
{
    QCustomPlot *plot = m_plotList.value(block);
    QCPItemTracer *tracer = m_pTracer.value(block);
    QCPItemText *text = m_pText.value(block);
    QCPCurve* curve = NULL;
    bool isCurve = false;

    if (plot == NULL || (plot->graphCount() == 0 && plot->plottableCount() == 0))
    {
        return;
    }

    //获取鼠标坐标，相对父窗体坐标
    Bit32 x_pos = event->pos().x();
    Bit32 y_pos = event->pos().y();

    //鼠标坐标转化为CustomPlot内部坐标
    fBit32 x_val = plot->xAxis->pixelToCoord(x_pos);
    fBit32 y_val = plot->yAxis->pixelToCoord(y_pos);

    //通过坐标轴范围判断光标是否在点附近
    fBit32 x_begin = plot->xAxis->range().lower;
    fBit32 x_end = plot->xAxis->range().upper;
    fBit32 y_begin = plot->yAxis->range().lower;
    fBit32 y_end = plot->yAxis->range().upper;
    fBit32 x_tolerate = (x_end - x_begin) / 40;//光标与最近点距离在此范围内，便显示该最近点的值
    fBit32 y_tolerate = (y_end - y_begin) / 40;

    //判断有没有超出坐标轴范围
    if (x_val < x_begin || x_val > x_end)
    {
        text->setVisible(false);
        return;
    }

    //通过x值查找离曲线最近的一个key值索引
    Bit32 index = 0;
    Bit32 index_left = 0;//左边最近的一个key值索引
    Bit32 index_right = 0;//右边
    fBit32 dif_left = 0;
    fBit32 dif_right = 0;
    if (plot->graphCount() == 0 && plot->plottableCount() != 0)
    {
        curve = qobject_cast<QCPCurve*>(plot->plottable(0));
        if (curve == NULL)
        {
            return;
        }
        isCurve = true;
        index_left = curve->findBegin(x_val, true);//左边最近的一个key值索引
        index_right = curve->findEnd(x_val, true);//右边
        dif_left = fabs(curve->data()->at(index_left)->key - x_val);
        dif_right = fabs(curve->data()->at(index_right)->key - x_val);

    }
    else
    {
        index_left = plot->graph(0)->findBegin(x_val, true);//左边最近的一个key值索引
        index_right = plot->graph(0)->findEnd(x_val, true);//右边
        dif_left = fabs(plot->graph(0)->data()->at(index_left)->key - x_val);
        dif_right = fabs(plot->graph(0)->data()->at(index_right)->key - x_val);
    }
    index = ((dif_left < dif_right) ? index_left : index_right);

    double x_posval = 0;
    double y_posval = 0;
    if (isCurve)
    {
        x_posval = curve->data()->at(index)->key;//通过得到的索引获取key值
        y_posval = curve->data()->at(index)->value;//通过得到的索引获取value值
    }
    else
    {
        x_posval = plot->graph(0)->data()->at(index)->key;//通过得到的索引获取key值
        y_posval = plot->graph(0)->data()->at(index)->value;//通过得到的索引获取value值
    }

    fBit32 dx = fabs(x_posval - x_val);
    fBit32 dy = fabs(y_posval - y_val);

    Bit32 graphIndex = 0;//curve index closest to the cursor
    //通过遍历每条曲线在index处的value值，得到离光标点最近的value及对应曲线索引
    if (isCurve)
    {
        for (Bit32 i = 0, n = plot->plottableCount(); i < n; i++)
        {
            QCPCurve *curveT = qobject_cast<QCPCurve*>(plot->plottable(0));
            if (curveT == NULL)
            {
                continue;
            }
            y_posval = fabs(curveT->data()->at(index)->value - y_val);
            if (y_posval < dy)
            {
                dy = y_posval;
                graphIndex = i;
            }
        }
    }
    else
    {
        for (Bit32 i = 0, n = plot->xAxis->graphs().count(); i < n; i++)
        {
            y_posval = fabs(plot->graph(i)->data()->at(index)->value - y_val);
            if (y_posval < dy)
            {
                dy = y_posval;
                graphIndex = i;
            }
        }
    }

//    ComDebug(QString("dx=%1,x_tolerate=%2,dy=%3,y_tolerate=%4").arg(dx).arg(x_tolerate).arg(dy).arg(y_tolerate));
    //判断光标点与最近点的距离是否在设定范围内
    if (dx <= x_tolerate && dy <= y_tolerate)
    {

        if (isCurve)
        {
            QCPCurve *curveT = qobject_cast<QCPCurve*>(plot->plottable(graphIndex));
            if (curveT == NULL)
            {
                return;
            }
            y_posval = curveT->data()->at(index)->value;
            tracer->setGraph(nullptr);
            tracer->position->setCoords(x_posval, y_posval);
        }
        else
        {
            y_posval = plot->graph(graphIndex)->data()->at(index)->value;
            tracer->setGraph(plot->graph(graphIndex));
            tracer->setGraphKey(x_posval);
        }
        tracer->setVisible(true);
        text->position->setParentAnchor(tracer->position);

        QString strToolTip = QString("x=%1\ny=%2").arg(x_posval).arg(y_posval);
        text->setText(strToolTip);
        text->setTextAlignment(Qt::AlignLeft);
        text->setPositionAlignment(Qt::AlignBottom | Qt::AlignHCenter);  // 文本底部对齐到 tracer
        text->position->setCoords(0, -10);
        text->setVisible(true);
    }
    else
    {
        text->setVisible(false);
        tracer->setVisible(false);

    }
    plot->replot();
}

void UnionPlot::keyPressEvent(QKeyEvent *event)
{
    bool handled = false;
    if (event->modifiers() & Qt::ControlModifier)
    {
        if (event->key() == Qt::Key_Z)
        {
            RePlot();
            handled = true;
        }
        else if (event->key() == Qt::Key_Plus)
        {
            EnlargeGraph(m_nCurBlock, 0);
            handled = true;
        }
        else if (event->key() == Qt::Key_Minus)
        {
            ReduceGraph(m_nCurBlock, 0);
            handled = true;

        }
        else if (event->key() == Qt::Key_F)
        {
            for (Bit32 i = 0; i < m_plotList.count(); i++)
            {
                if (i != m_nCurBlock)
                {
                    m_plotList.at(i)->setVisible(false);

                }
            }
            handled = true;
        }
        else if (event->key() == Qt::Key_I)
        {
            QList<QPair<fBit64, fBit64>> rangeList = m_pDlg->ExecAndRet();
            RedrawVline(rangeList);
            handled = true;
        }
    }
    else if (event->key() == Qt::Key_Plus)
    {
        EnlargeGraph(m_nCurBlock, 1);
        handled = true;
    }
    else if (event->key() == Qt::Key_Minus)
    {
        ReduceGraph(m_nCurBlock, 1);
        handled = true;
    }

    if (handled)
        event->accept(); 
    else
        event->ignore();   //未处理 → 交给父类
}

void UnionPlot::EnlargeGraph(Bit32 block, Bit16 axis, fBit64 scaleFactor)
{
    if (block < 0 || block >= m_plotList.count())
    {
        return;
    }
    QCustomPlot * plot = m_plotList.at(block);
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

void UnionPlot::ReduceGraph(Bit32 block, Bit16 axis, fBit64 scaleFactor)
{
    if (block < 0 || block >= m_plotList.count())
    {
        return;
    }
    QCustomPlot * plot = m_plotList.at(block);
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

void UnionPlot::FitRange(QPair<fBit64, fBit64> &first, QPair<fBit64, fBit64> second)
{
    if (first.first > second.first)
    {
        first.first = second.first;
    }
    if (first.second < second.second)
    {
        first.second = second.second;
    }
}

void UnionPlot::RedrawVline(QList<QPair<fBit64, fBit64> > rangeList)
{
    foreach(QCPItemStraightLine *line, m_vlineList)
    {
        line->setParent(NULL);
        delete line;
    }
    m_vlineList.clear();
    for (Bit32 i = 0; i < m_plotList.count(); i++)
    {
        QCustomPlot *plot = m_plotList.at(i);
        for (Bit32 j = 0; j < rangeList.count(); j++)
        {
            QCPItemStraightLine *vline1 = new QCPItemStraightLine(plot);
            m_vlineList << vline1;
            vline1->point1->setCoords(rangeList.at(j).first, 0);
            vline1->point2->setCoords(rangeList.at(j).first, 1);
            vline1->setPen(QColor(Qt::magenta));

            QCPItemStraightLine *vline2 = new QCPItemStraightLine(plot);
            m_vlineList << vline2;
            vline2->point1->setCoords(rangeList.at(j).second, 0);
            vline2->point2->setCoords(rangeList.at(j).second, 1);
            vline2->setPen(QColor(Qt::darkMagenta));
        }
        plot->replot();
    }
}
