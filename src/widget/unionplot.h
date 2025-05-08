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
    void SetData(QList<QVector<fBit64>> xdata, QList<QVector<fBit64>> ydata);
    void SetAxisLabel(QStringList xlabelList, QStringList ylabelList);
    void RePlot();

private:
    Ui::UnionPlot *ui;
    QList<QCustomPlot*> m_plotList;
    QList<QPair<fBit64, fBit64>> m_xRangeList;
    QList<QPair<fBit64, fBit64>> m_yRangeList;
};

#endif // UNIONPLOT_H
