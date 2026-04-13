#ifndef DATAPAINTER_H
#define DATAPAINTER_H

#include <QWidget>

namespace Ui {
class DataPainter;
}

class DataPainter : public QWidget
{
    Q_OBJECT

public:
    static DataPainter& GetInstance();
    ~DataPainter();

    QString GetStyleSheetStr(){return m_sStyleSheet;} // 获取qss样式

private:
    Ui::DataPainter *ui;
    QString m_sStyleSheet;

    explicit DataPainter(QWidget *parent = nullptr);
    void Init();
};

#endif // DATAPAINTER_H
