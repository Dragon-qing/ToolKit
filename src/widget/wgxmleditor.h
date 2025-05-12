#ifndef WGXMLEDITOR_H
#define WGXMLEDITOR_H

#include <QStandardItemModel>
#include <QStringListModel>

#include "basewidget.h"
#include "hmixmleditor.h"
#include "dlgprompt.h"

namespace Ui {
class WgXmlEditor;
}

class WgXmlEditor : public BaseWidget
{
    Q_OBJECT

public:
    explicit WgXmlEditor(QWidget *parent = nullptr);
    ~WgXmlEditor();

private:
    Ui::WgXmlEditor *ui;
    QStandardItemModel *m_pTreeModel;
    QStringListModel *m_pListModel;
    QModelIndex m_currentIndex;
    DlgPrompt *m_pDlg;


    void InitModel();

private slots:
    void on_pushButton_clicked();
    void on_selectBtn_clicked();
    void HandleTreeViewClicked(const QModelIndex &index);
    void HandleListDataChange(const QModelIndex &topLeft, const QModelIndex &bottomRight);
    void on_saveBtn_clicked();
};

#endif // WGXMLEDITOR_H
