#include "QMessageBox"
#include "QDebug"
#include "QFileDialog"
#include "QPushButton"

#include "wggeneratebugfolder.h"
#include "ui_wggeneratebugfolder.h"

WgGenerateBugFolder::WgGenerateBugFolder(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::WgGenerateBugFolder)
{
    ui->setupUi(this);
    m_sSrcPath = "";
    m_sStatementPath = "";
    m_sDirPath = "";
    m_bIsSaveUI = false;
    m_pClassNameVector = new QVector<QString>();

    connect(ui->btnSrcPath, SIGNAL(clicked()),this, SLOT(BtnSrcPathClicked()));
    connect(ui->btnStatementPath, SIGNAL(clicked()),this, SLOT(BtnStatementClicked()));
    connect(ui->btnDirPath, SIGNAL(clicked()),this, SLOT(BtnDirPathClicked()));
    connect(ui->btnStart, SIGNAL(clicked()),this, SLOT(BtnDirPathClicked()));
    connect(ui->checkBox, SIGNAL(stateChanged(int)), this, SLOT(CkbIsSaveUI(int)));
}

WgGenerateBugFolder::~WgGenerateBugFolder()
{
    if(m_pClassNameVector != nullptr)
    {
        delete m_pClassNameVector;
        m_pClassNameVector = nullptr;
    }
    delete ui;
}

void WgGenerateBugFolder::BtnSrcPathClicked()
{
    m_sSrcPath = QFileDialog::getExistingDirectory(this, "选择源码目录", QDir::homePath()).trimmed();

    ui->leSrcPath->setText(m_sSrcPath);
}

void WgGenerateBugFolder::BtnStatementClicked()
{
    m_sStatementPath = QFileDialog::getExistingDirectory(this, "选择文档目录", QDir::homePath()).trimmed();

    ui->leStatementPath->setText(m_sStatementPath);
}

void WgGenerateBugFolder::BtnDirPathClicked()
{
    m_sDirPath = QFileDialog::getExistingDirectory(this, "选择目标目录", QDir::homePath()).trimmed();

    ui->leDirPath->setText(m_sDirPath);
}

void WgGenerateBugFolder::ErrorMessage(const char *message)
{
    QMessageBox::critical(this, "错误", message);
}

void WgGenerateBugFolder::DataCheck()
{
    if(m_sClassName == "")
    {
        ErrorMessage("类名不能为空！！！");
    }
    if(m_sSrcPath == "")
    {
        ErrorMessage("源文件目录不能为空！！！");
    }
    if(m_sDirPath == "")
    {
        ErrorMessage("目标路径不能为空！！！");
    }
    if(m_bIsSaveUI == true && m_sStatementPath == "")
    {
        ErrorMessage("说明文档路径不能为空！！！");
    }
}

void WgGenerateBugFolder::BtnStartClicked()
{
    if(CheckFolder(m_sSrcPath) == false)
    {
        ErrorMessage("源码目录错误或为空！！！");
    }
    else if(CheckFolder(m_sStatementPath) == false)
    {
        ErrorMessage("说明文档目录错误或为空！！！");
    }
    else if(CheckFolder(m_sDirPath) == false)
    {
        ErrorMessage("目标目录错误或为空！！！");
    }
}

void WgGenerateBugFolder::CkbIsSaveUI(int arg1)
{
    m_bIsSaveUI = arg1 == 0?false:true;
    qDebug()<<m_bIsSaveUI<<endl;
}

bool WgGenerateBugFolder::CheckFolder(QString path)
{
    QFileInfo folder(path);
    return folder.exists();
}
