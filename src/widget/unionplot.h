#ifndef UNIONPLOT_H
#define UNIONPLOT_H

#include <QList>

#include "basewidget.h"
#include "qcustomplot.h"

namespace Ui {
class UnionPlot;
}

class UnionPlot : public BaseWidget
{
    Q_OBJECT

public:
    typedef enum _Layout_Type{
        HORIZON_LAYOUT = 0,     // 水平排列
        VERTICAL_LAYOUT,        // 垂直排列
        RECTANGLE_LAYOUT        // 矩阵排列

    }Layout_Type;

    explicit UnionPlot(Layout_Type layout, Bit32 plotNum, QWidget *parent = nullptr);
    ~UnionPlot();
    void SetData(QList<QVector<fBit64>> *xdata, QList<QVector<fBit64>> *ydata, QVector<Bit32> graphNum, QStringList lineName = QStringList());
    void SetAxisLabel(QStringList xlabelList, QStringList ylabelList);
    void RePlot();
    bool eventFilter(QObject *obj, QEvent *event) override;

public slots:
    void MouseMoveHandleSlot(QMouseEvent *event, Bit32 block); // 鼠标移动事件处理

protected:
    void keyPressEvent(QKeyEvent *event) override;

private:
    Ui::UnionPlot *ui;
    QList<QCustomPlot*> m_plotList;
    QList<QPair<fBit64, fBit64>> m_xRangeList;
    QList<QPair<fBit64, fBit64>> m_yRangeList;
    Bit32 m_nPreBlock;
    Bit32 m_nCurBlock;
    QList<QCPItemTracer*> m_pTracer;
    QList<QCPItemText*> m_pText;

    void EnlargeGraph(Bit32 block, Bit16 axis); // 放大图像
    void ReduceGraph(Bit32 block, Bit16 axis); // 缩小图像
    void FitRange(QPair<fBit64, fBit64> &first, QPair<fBit64, fBit64>second);
};

#endif // UNIONPLOT_H
