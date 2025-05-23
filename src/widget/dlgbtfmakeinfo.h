#ifndef DLGBTFMAKEINFO_H
#define DLGBTFMAKEINFO_H

#include <QDialog>

#include "basetable.h"

QT_BEGIN_NAMESPACE
class InfoTable;
QT_END_NAMESPACE

namespace Ui {
class DlgBTFMakeInfo;
}

class DlgBTFMakeInfo : public QDialog
{
    Q_OBJECT

public:
    explicit DlgBTFMakeInfo(QWidget *parent = nullptr);
    ~DlgBTFMakeInfo();
    void SetFiles(QStringList list);
    QStringList Exec();

private:
    Ui::DlgBTFMakeInfo *ui;
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
#endif // DLGBTFMAKEINFO_H
