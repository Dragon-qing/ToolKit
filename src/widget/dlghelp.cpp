#include "common.h"

#include "dlghelp.h"
#include "ui_dlghelp.h"

DlgHelp::DlgHelp(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::DlgHelp)
{
    ui->setupUi(this);
    setWindowFlag(Qt::WindowContextHelpButtonHint, false);
}

DlgHelp::~DlgHelp()
{
    delete ui;
}

void DlgHelp::SetContent(QStringList content)
{
    QFont font(FONT_STYLE, 10);
    QString display = "";

    ui->textBrowser->setFont(font);
    foreach (QString str, content)
    {
        display += QString("<p>%1</p>").arg(str);
    }
    if (display.isEmpty())
    {
        display = QObject::TR("帮助文档暂时为空，等待补充。");
    }
    ui->textBrowser->setHtml(display);
}
