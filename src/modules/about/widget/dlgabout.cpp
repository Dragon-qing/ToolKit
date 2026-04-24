#include <QPainter>
#include <QStyleOption>

#include "sysapi.h"

#include "common.h"

#include "dlgabout.h"
#include "ui_dlgabout.h"

// 私有类
class DlgAboutPrivate
{
public:
    DlgAboutPrivate()
    {
        m_sContent = "";
        InitContent();
    }

    /**
     * @brief: 添加内容
     * @param {QString} &title: 标题
     * @param {QString} &detail: 详细内容
     * @return {void}
     */
    void AddContent(const QString &title, const QString &detail)
    {
        m_sContent += QString("<p><b>%1</b>:%2</p>").arg(title, detail);
    }

    /**
     * @brief: 获取文本数据
     * @return {QString} 文本字符串
     */
    QString GetContent()
    {
        return m_sContent;
    }

private:
    QString m_sContent; // 文本内容

    /**
     * @brief: 初始化文本
     * @return {*}
     */
    void InitContent()
    {
        AddContent(QObject::TR("软件版本"), SYS_VERSION);
    }
};

DlgAbout::DlgAbout(QWidget *parent)
    : QDialog(parent),
     ui(new Ui::DlgAbout),
     d_ptr(new DlgAboutPrivate)
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
