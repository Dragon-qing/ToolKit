#ifndef WGCOMRPT_H
#define WGCOMRPT_H

#include "basewidget.h"
#include "dlgprompt.h"
#include "unionplot.h"

namespace Ui {
class WgComRpt;
}

class WgComRpt : public BaseWidget
{
    Q_OBJECT

public:
    explicit WgComRpt(QWidget *parent = nullptr);
    ~WgComRpt();

protected:
    void keyPressEvent(QKeyEvent *event) override;

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

    void InitDict();
    void UnionReplot();
    void BuildUnioPlot(QString mask);
    void BuildJourDefaultPlot();
    void BuildPosDefaultPlot();
    
    void AddUnionPlot(UnionPlot *plot);

private slots:
    void on_OpenBtn_clicked();
    void ProcessChangeHandle(int changeValue);
    void MouseMoveEventHandle(QMouseEvent *event);
    void on_drawBtn_clicked();
    void DefaultRadioHandle(bool checked);
};

#endif // WGCOMRPT_H
