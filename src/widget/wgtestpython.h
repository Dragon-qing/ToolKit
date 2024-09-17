#ifndef WGTESTPYTHON_H
#define WGTESTPYTHON_H

#include <QWidget>

namespace Ui {
class WgTestPython;
}

class WgTestPython : public QWidget
{
    Q_OBJECT

public:
    explicit WgTestPython(QWidget *parent = nullptr);
    ~WgTestPython();

private:
    Ui::WgTestPython *ui;

private slots:
    void TestPy();
};

#endif // WGTESTPYTHON_H
