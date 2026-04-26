/*
 * @Author: Dragon-qing
 * @Date: 2026-04-18
 * @LastEditors: Dragon-qing
 * @FilePath: \ToolKit\src\modules\archive\widget\dlgarchivemakeinfo.h
 * @Description: 详情文件弹窗
 */
#ifndef DLGARCHIVEMAKEINFO_H
#define DLGARCHIVEMAKEINFO_H

#include <QDialog>

#include "basetable.h"

QT_BEGIN_NAMESPACE
class InfoTable;
QT_END_NAMESPACE

namespace Ui {
class DlgArchiveMakeInfo;
}

class DlgArchiveMakeInfo : public QDialog
{
    Q_OBJECT

public:
    explicit DlgArchiveMakeInfo(QWidget *parent = nullptr);
    ~DlgArchiveMakeInfo();
    void SetFiles(QStringList& list);
    QStringList Exec();

private:
    Ui::DlgArchiveMakeInfo *ui;
    InfoTable *m_pTab;
};

class InfoTable : public BaseTable
{
    Q_OBJECT
public:
    enum _COL_TYEP{
        INDEX_COL = 0,
        PATH_COL,

        COL_NUM
    };
    explicit InfoTable(QWidget *parent = nullptr);
    ~InfoTable();

    QVariant ContentGet(Bit32 row, Bit32 col) override;
    Bit32 TotalRow() override;
    void SetFilePath(QStringList list);
    QStringList GetFilesPath(){ return m_filePathList; }
protected:
    void keyPressEvent(QKeyEvent *event) override;
private:
    QStringList m_filePathList;

};
#endif // DLGARCHIVEMAKEINFO_H
