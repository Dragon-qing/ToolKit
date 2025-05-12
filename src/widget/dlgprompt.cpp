/*!
 * @brief 提示弹框
 * @file dlgprompt.cpp
 * @author Dragon_qing
 * @date 2025/04/25
 */

#include "dlgprompt.h"
#include "ui_dlgprompt.h"

void DlgPrompt::InitUi()
{
    resize(DLG_WIDTH, DLG_HEIGHT);
    this->setWindowTitle(TR("提示"));
    this->setWindowFlags(windowFlags() & ~Qt::WindowContextHelpButtonHint);

    m_pButtonGroup = new QButtonGroup(this);
    m_pButtonGroup->addButton(ui->ok_Btn, OK_BUTTON);
    m_pButtonGroup->addButton(ui->cancel_Btn, CANCEL_BUTTON);
    connect(m_pButtonGroup, QOverload<int>::of(&QButtonGroup::buttonClicked), this, &DlgPrompt::HandleClicked);
    ui->ok_Btn->setVisible(false);
    ui->cancel_Btn->setVisible(false);
}

void DlgPrompt::HandleClicked(int id)
{
    if (id == OK_BUTTON)
    {
        this->accept();
        m_nCurrentCode = OK_CODE;
    }
    else if (id == CANCEL_BUTTON)
    {
        this->reject();
        m_nCurrentCode = CANCEL_CODE;
    }
}

DlgPrompt::DlgPrompt(Bit32 buttons, QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::DlgPrompt)
{
    ui->setupUi(this);

    InitUi();

    if (buttons & OK_BUTTON)
    {
        ui->ok_Btn->setVisible(true);
    }
    if (buttons & CANCEL_BUTTON)
    {
        ui->cancel_Btn->setVisible(true);
    }
}

DlgPrompt::~DlgPrompt()
{
    delete ui;
}

Bit32 DlgPrompt::ExecAndRet(const QString &context)
{
    ui->label->setText(context);
    this->exec();
    return m_nCurrentCode;
}
