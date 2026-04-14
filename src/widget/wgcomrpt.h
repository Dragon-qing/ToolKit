#ifndef WGCOMRPT_H
#define WGCOMRPT_H

#include "basewidget.h"
#include "dlgprompt.h"
#include "unionplot.h"
#include "dlgcomres.h"

namespace Ui {
class WgComRpt;
}

class WgComRpt : public BaseWidget
{
    Q_OBJECT

public:
    explicit WgComRpt(QWidget *parent = nullptr);
    ~WgComRpt();

    void MessageFlows(QVariant messageid, QVariant messageValue) override;

protected:
    void keyPressEvent(QKeyEvent *event) override;
    bool eventFilter(QObject *watched, QEvent *event) override;

private:
    enum _AXIS_TYPE{
        TIME = 0,

        AXIS_TYPE_NUM
    };
    Ui::WgComRpt *ui;
    DlgPrompt *m_pDlg;
    QPair<fBit64, fBit64> m_xAxisRange;
    QPair<fBit64, fBit64> m_yAxisRange;
    QMap<QString, QString> *m_pDictMap;
    QCPItemTracer *m_pTracer;
    QCPItemText *m_pQCPText;

    // 组合图像
    UnionPlot *m_pCurPlot; // 当前图像
    UnionPlot *m_pJourPlot; // 全程电流
    UnionPlot *m_pPosPlot; // 位置环
    UnionPlot *m_pSpeedPlot; // 速度环
    UnionPlot *m_pSpdlDirPlot; // 主轴定向
    UnionPlot *m_pSpdlFluPlot; // 主轴波动
    UnionPlot *m_pSpdlSpeedPlot; // 主轴升降速
    UnionPlot *m_pToolChangePlot; // 换刀
    UnionPlot *m_pTapPlot; // 刚性攻丝
    UnionPlot *m_pCirclePlot; // 圆度测试

    QString m_sOpenFolder; // 文件打开路径

    DlgComRes *m_pDlgComRes;

    void InitDict();
    void UnionReplot();
    void BuildUnioPlot(QString mask);
    void BuildJourDefaultPlot();
    void BuildPosDefaultPlot();
    void BuildSpdlDirDefaultPlot();
    void BuildSpdlFluDefaultPlot();
    void BuildSpeedDefaultPlot();
    void BuildSpdlSpeedDefultPlot();
    void BuildToolChangeDefaultPlot();
    void BuildTapDefault();
    void BuildCircleDefault();

    void AddUnionPlot(UnionPlot *plot);
    QStringList GetHelpText() override;
    void EnlargeGraph(Bit16 axis, fBit64 scaleFactor = 0.9);
    void ReduceGraph(Bit16 axis, fBit64 scaleFactor = 1.1);

private slots:
    void on_OpenBtn_clicked();
    void ProcessChangeHandle(int changeValue);
    void MouseMoveEventHandle(QMouseEvent *event);
    void on_drawBtn_clicked();
    void DefaultRadioHandle(bool checked);
};

#endif // WGCOMRPT_H
