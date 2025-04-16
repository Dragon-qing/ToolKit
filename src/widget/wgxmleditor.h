#ifndef WGXMLEDITOR_H
#define WGXMLEDITOR_H

#include <QStandardItemModel>

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
    QStandardItemModel *m_pModel;

    void InitModel();

private slots:
    void on_pushButton_clicked();
};

#endif // WGXMLEDITOR_H
