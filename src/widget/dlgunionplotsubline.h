#ifndef DLGUNIONPLOTSUBLINE_H
#define DLGUNIONPLOTSUBLINE_H

#include <QDialog>
#include <QCheckBox>
#include <QLineEdit>

#include "datadef.h"

namespace Ui {
class DlgUnionPlotSubline;
}

class DlgUnionPlotSubline : public QDialog
{
    Q_OBJECT

public:
    explicit DlgUnionPlotSubline(QWidget *parent = nullptr);
    ~DlgUnionPlotSubline();
    QList<QPair<fBit64, fBit64>>ExecAndRet();

private:
    Ui::DlgUnionPlotSubline *ui;

    QList<QCheckBox*> m_enableList;
    QList<QLineEdit*> m_stList;
    QList<QLineEdit*> m_edList;

    // QWidget interface
protected:
    void keyPressEvent(QKeyEvent *event) override;
};

#endif // DLGUNIONPLOTSUBLINE_H
