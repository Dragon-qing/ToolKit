#ifndef WGWEBBOWSER_H
#define WGWEBBOWSER_H

#include "basewidget.h"
#include "tkwebview.h"

namespace Ui {
class WgWebBowser;
}

class WgWebBowser : public BaseWidget
{
    Q_OBJECT

public:
    explicit WgWebBowser(QWidget *parent = nullptr);
    ~WgWebBowser();

private:
    Ui::WgWebBowser *ui;
    TKWebView *m_pWebView;
};

#endif // WGWEBBOWSER_H
