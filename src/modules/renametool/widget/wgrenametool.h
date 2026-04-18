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

    // BaseWidget interface
public:
    void MessageFlows(QVariant messageid, QVariant messageValue) override;
    QStringList GetHelpText() override;

private:
    Ui::WgRenameTool *ui;
};

#endif // WGRENAMETOOL_H
