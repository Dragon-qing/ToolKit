#include "wgxmleditor.h"
#include "ui_wgxmleditor.h"

WgXmlEditor::WgXmlEditor(QWidget *parent)
    : BaseWidget(parent)
    , ui(new Ui::WgXmlEditor)
{
    ui->setupUi(this);
}

WgXmlEditor::~WgXmlEditor()
{
    delete ui;
}
