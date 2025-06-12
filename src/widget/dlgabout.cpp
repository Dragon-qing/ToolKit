#include <QPainter>
#include <QStyleOption>

#include "sysapi.h"

#include "common.h"

#include "dlgabout.h"
#include "ui_dlgabout.h"

DlgAbout::DlgAbout(QWidget *parent)
    : QDialog(parent), d_ptr(new DlgAboutPrivate)
    , ui(new Ui::DlgAbout)
{
    ui->setupUi(this);
    setWindowTitle(QObject::TR("关于"));
    setWindowFlag(Qt::WindowContextHelpButtonHint, false);
    Init();
}

DlgAbout::~DlgAbout()
{
    delete ui;
}

void DlgAbout::Init()
{
    QFont font(FONT_STYLE, 10);

    ui->textBrowser->setFont(font);
    Refresh();
}

void DlgAbout::Refresh()
{
    Q_D(DlgAbout);
    QString str = d->GetContent();

    ui->textBrowser->setHtml(str);
}

DlgAboutPrivate::DlgAboutPrivate()
{
    m_sContent = "";
    InitContent();
}

void DlgAboutPrivate::AddContent(const QString &title, const QString &detail)
{
    m_sContent += QString("<p><b>%1</b>:%2</p>").arg(title, detail);
}

QString DlgAboutPrivate::GetContent()
{
    return m_sContent;
}

void DlgAboutPrivate::InitContent()
{
    AddContent(QObject::TR("软件版本"), SYS_VERSION);
}
