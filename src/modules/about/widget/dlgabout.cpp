#include <QPainter>
#include <QStyleOption>
#include <QDesktopServices>
#include <QUrl>
#include <QPainter>


#include "sysapi.h"

#include "common.h"

#include "dlgabout.h"
#include "ui_dlgabout.h"

DlgAbout::DlgAbout(QWidget *parent)
    : QDialog(parent),
     ui(new Ui::DlgAbout)
{
    ui->setupUi(this);
    resize(800, 600);
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
    ui->frame->setAttribute(Qt::WA_TranslucentBackground);
    ui->frame_func->setAttribute(Qt::WA_TranslucentBackground);
    ui->label_wrench->setPixmap(QPixmap(":/img/about-wrench.jpg").scaled(256, 256, Qt::KeepAspectRatio, Qt::SmoothTransformation));

    ui->label_version_rect->setFont(QFont(FONT_STYLE, 9));
    ui->label_version_rect->setAlignment(Qt::AlignCenter);
    ui->label_version_rect->setText(SYS_VERSION);
    ui->label_version->setText(SYS_VERSION);

    QGridLayout* grid = ui->gridLayout;
    for (int i = 0; i < grid->count(); ++i) {
        QWidget* w = grid->itemAt(i)->widget();
        if (auto label = qobject_cast<QLabel*>(w)) {
            label->setFont(font);
        }
    }

    ui->label_link->setOpenExternalLinks(false); // 禁止Qt内部自动打开链接
    ui->label_github->setPixmap(RenderSvg(":/img/github.svg", 24, 24));
    ui->label_link->setText(R"(
        <a href="https://github.com/Dragon-qing/ToolKit">
            github.com/Dragon-qing/ToolKit
        </a>
    )");
    connect(ui->label_link, &QLabel::linkActivated, this, [](const QString& url){
        QDesktopServices::openUrl(QUrl(url));
    });

}

void DlgAbout::on_pushButton_close_clicked()
{
    this->close();
}
