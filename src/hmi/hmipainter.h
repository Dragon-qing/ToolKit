#ifndef HMIPAINTER_H
#define HMIPAINTER_H

#include <QWidget>

namespace Ui {
class HmiPainter;
}

class HmiPainter : public QWidget
{
    Q_OBJECT

public:
    static HmiPainter& GetInstance();
    ~HmiPainter();

    QString GetStyleSheetStr(){return m_sStyleSheet;} // 获取qss样式

private:
    Ui::HmiPainter *ui;
    QString m_sStyleSheet;

    explicit HmiPainter(QWidget *parent = nullptr);
    void Init();
};

#endif // HMIPAINTER_H
