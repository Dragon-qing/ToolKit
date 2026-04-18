#ifndef WGRENAMETOOL_H
#define WGRENAMETOOL_H

#include "basewidget.h"

namespace Ui {
class WgRenameTool;
}

class WgRenameTool : public BaseWidget
{
    Q_OBJECT

public:
    explicit WgRenameTool(QWidget *parent = nullptr);
    ~WgRenameTool();

private:
    Ui::WgRenameTool *ui;
};

#endif // WGRENAMETOOL_H
