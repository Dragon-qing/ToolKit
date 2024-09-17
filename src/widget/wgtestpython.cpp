#include "Python.h"
#include "common.h"
#include "wgtestpython.h"
#include "ui_wgtestpython.h"

WgTestPython::WgTestPython(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::WgTestPython)
{
    ui->setupUi(this);
    connect(ui->pushButton, SIGNAL(clicked()), this, SLOT(TestPy()));
}

WgTestPython::~WgTestPython()
{
    delete ui;
}

void WgTestPython::TestPy()
{
    // 初始化解释器
    Py_Initialize();
    if (!Py_IsInitialized())
    {
        ComDebug(TR("初始化失败"));
    }
    else
    {
        ComDebug(TR("初始化成功"));
    }
    // 执行python语句
    PyRun_SimpleString("print('hello world')");
    // 销毁解释器
    Py_Finalize();
}
