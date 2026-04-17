#include <QKeyEvent>

#include "common.h"

#include "dlgarchivemakeinfo.h"
#include "ui_dlgarchivemakeinfo.h"

DlgArchiveMakeInfo::DlgArchiveMakeInfo(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::DlgArchiveMakeInfo)
{
    ui->setupUi(this);
    setWindowFlag(Qt::WindowContextHelpButtonHint, false);

    m_pTab = new InfoTable(this);
    ui->verticalLayout->addWidget(m_pTab);
    setMinimumSize(DLG_WIDTH, DLG_HEIGHT);
    adjustSize();
}

DlgArchiveMakeInfo::~DlgArchiveMakeInfo()
{
    delete ui;
}

void DlgArchiveMakeInfo::SetFiles(QStringList list)
{
    m_pTab->SetFilePath(list);
}

QStringList DlgArchiveMakeInfo::Exec()
{
    m_pTab->ReDraw();
    exec();
    return m_pTab->GetFilesPath();
}

InfoTable::InfoTable(QWidget *parent)
    : BaseTable(parent)
{
    m_filePathList.clear();
    QVector<Bit32> scale;
    scale << 1 << 7;
    QStringList headList;
    headList << TR("序号") << TR("文件");

    InitTable(headList, scale);
}

InfoTable::~InfoTable()
{

}

QVariant InfoTable::ContentGet(Bit32 row, Bit32 col)
{
    if (col == INDEX_COL)
    {
        return QString::number(row + 1);
    }
    else if (col == PATH_COL)
    {
        if (row < 0 || row > m_filePathList.count())
        {
            return QVariant();
        }
        return m_filePathList.at(row);
    }

    return QVariant();
}

Bit32 InfoTable::TotalRow()
{
    return m_filePathList.count();
}

void InfoTable::SetFilePath(QStringList list)
{
    m_filePathList = list;
}

void InfoTable::keyPressEvent(QKeyEvent *event)
{
    if (event->modifiers() & Qt::ControlModifier && event->key() == Qt::Key_D || event->key() == Qt::Key_Delete)
    {
        QModelIndexList indexList = selectedIndexes();
        // 排序
        std::sort(indexList.begin(), indexList.end(), [](const QModelIndex &a, const QModelIndex &b) {
            return a.row() > b.row(); // 从大到小排序
        });
        QSet<Bit32> rowSet;
        for (Bit32 i = 0; i < indexList.count(); i++)
        {
            if (rowSet.contains(indexList.at(i).row()))
            {
                continue;
            }
            else
            {
                rowSet.insert(indexList.at(i).row());
                m_pModel->removeRow(indexList.at(i).row());
                m_filePathList.removeAt(indexList.at(i).row());
            }

        }
        ReDraw();
    }
    BaseTable::keyPressEvent(event);
}
