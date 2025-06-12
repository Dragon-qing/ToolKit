#ifndef DLGABOUT_H
#define DLGABOUT_H

#include <QDialog>
#include <QScopedPointer>

namespace Ui {
class DlgAbout;
}

class DlgAboutPrivate;

class DlgAbout : public QDialog
{
    Q_DECLARE_PRIVATE(DlgAbout)
    Q_OBJECT

public:
    explicit DlgAbout(QWidget *parent = nullptr);
    ~DlgAbout();

private:
    Ui::DlgAbout *ui;
    QScopedPointer<DlgAboutPrivate> d_ptr; // d指针，使用智能指针，自动释放；

    void Init();
    void Refresh();
};

class DlgAboutPrivate
{
public:
    DlgAboutPrivate();
    void AddContent(const QString &title, const QString &detail);
    QString GetContent();

private:
    QString m_sContent;

    void InitContent();
};

#endif // DLGABOUT_H
