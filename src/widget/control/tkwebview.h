#ifndef TKWEBVIEW_H
#define TKWEBVIEW_H

#include <QWebEngineView>

namespace Ui {
class TKWebView;
}

class TKWebView : public QWebEngineView
{
    Q_OBJECT

public:
    explicit TKWebView(QWidget *parent = nullptr);
    ~TKWebView();

protected:
    QWebEngineView *createWindow(QWebEnginePage::WebWindowType type) override;

private:
    Ui::TKWebView *ui;
};

#endif // TKWEBVIEW_H
