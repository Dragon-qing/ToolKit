#include "tkwebview.h"
#include "ui_tkwebview.h"

#include <QMainWindow>

TKWebView::TKWebView(QWidget *parent)
    : QWebEngineView(parent)
    , ui(new Ui::TKWebView)
{
    ui->setupUi(this);
}

TKWebView::~TKWebView()
{
    delete ui;
}

QWebEngineView *TKWebView::createWindow(QWebEnginePage::WebWindowType type)
{
    TKWebView *newView = new TKWebView();
    QMainWindow *newWindow = new QMainWindow();
    newWindow->setCentralWidget(newView);
    newWindow->resize(800, 600);
    newWindow->show();
    return newView;
}
