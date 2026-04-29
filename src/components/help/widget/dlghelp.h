#ifndef DLGHELP_H
#define DLGHELP_H

#include <QDialog>
#include <QStandardItemModel>

namespace Ui {
class DlgHelp;
}

class DlgHelp : public QDialog
{
    Q_OBJECT

public:
    explicit DlgHelp(QWidget *parent = nullptr);
    ~DlgHelp();
    void Refresh();

private:
    Ui::DlgHelp *ui;
    QStandardItemModel* m_pModel;

    void ClearModel();
private slots:
    void OnCategoryChanged(const QModelIndex &current, const QModelIndex &previous);
};


#endif // DLGHELP_H
