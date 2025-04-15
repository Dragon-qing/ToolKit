#ifndef WGXMLEDITOR_H
#define WGXMLEDITOR_H

#include "basewidget.h"
#include "hmixmleditor.h"

namespace Ui {
class WgXmlEditor;
}

class WgXmlEditor : public BaseWidget
{
    Q_OBJECT

public:
    explicit WgXmlEditor(QWidget *parent = nullptr);
    ~WgXmlEditor();

private:
    Ui::WgXmlEditor *ui;
};

#endif // WGXMLEDITOR_H
