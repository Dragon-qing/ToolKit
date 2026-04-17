/*!
 * @brief 压缩包制作页面
 * @file wgarchivemaker.cpp
 * @author Dragon_qing
 * @date 2024/10/20
 */

#include <QDragEnterEvent>
#include <QMimeData>
#include <QFileInfo>
#include <QMessageBox>
#include <QFileDialog>
#include <QProcess>
#include <QPainter>

#include "common.h"
#include "dataconfig.h"
#include "tklogger.h"
#include "toolfactory.h"

#include "wgarchivemaker.h"
#include "ui_wgarchivemaker.h"

WgArchiveMaker::WgArchiveMaker(QWidget *parent) :
    BaseWidget(parent),
    ui(new Ui::WgArchiveMaker),
    m_p7zTool(ToolFactory::Instance().CreateTool("7zip")),
    m_pMD5Tool(ToolFactory::Instance().CreateTool("md5ForBTF"))
{
    ui->setupUi(this);
    ui->label_top->setText("");

    m_labelList.clear();
    m_nameList.clear();
    m_pathList.clear();

    ui->lineEdit->setText(DataConfig::Instance().GetConfig("archive_cfg", "archive_savepath"));
    ui->lineEdit_2->setText(DataConfig::Instance().GetConfig("archive_cfg", "archive_savefilename"));
    int index = ui->comboBox_format->findText(DataConfig::Instance().GetConfig("archive_cfg", "archive_format"));
    if (index != -1)
    {
        ui->comboBox_format->setCurrentIndex(index);
    }

    m_pDlgInfo = new DlgArchiveMakeInfo(this);
    m_bHasMoreLabel = false;
    m_pMoreLabel = NULL;
    m_pDlgProcess = new DlgArchiveProcess(this);
    m_pDlgProcess->SetExternalTool(m_p7zTool.get());

    connect(ui->comboBox_format, &QComboBox::currentTextChanged, this, &WgArchiveMaker::OnFormatChangedSlot);
    setAcceptDrops(true);
}

WgArchiveMaker::~WgArchiveMaker()
{
    delete ui;
}

bool WgArchiveMaker::eventFilter(QObject *obj, QEvent *event)
{
    QLabel *label = dynamic_cast<QLabel *>(obj);
    if (event->type() == QEvent::MouseButtonDblClick)
    {
        QMouseEvent *mouseEvent = dynamic_cast<QMouseEvent *>(event);
        if (label != NULL && mouseEvent != NULL)
        {
            if (mouseEvent->button() == Qt::LeftButton)
            {
                RemoveItem(label);
                Refresh();
                if (m_labelList.isEmpty())
                {
                    ui->label->setVisible(true);
                }
                return true;
            }
        }
    }
    else if (event->type() == QEvent::MouseButtonPress)
    {
        QMouseEvent *mouseEvent = dynamic_cast<QMouseEvent *>(event);
        if (label != NULL && mouseEvent != NULL)
        {
            if (mouseEvent->button() == Qt::LeftButton)
            {
                if (label->property("isMore").toBool())
                {
                    m_pDlgInfo->SetFiles(m_pathList);
                    QStringList filesPath = m_pDlgInfo->Exec();
                }
                return true;
            }
        }
    }

    return QObject::eventFilter(obj, event);
}

void WgArchiveMaker::dragEnterEvent(QDragEnterEvent *event)
{
    if (event == NULL)
    {
        return;
    }
    const QMimeData *mime = event->mimeData();
    if (mime->hasUrls())
    {
        event->acceptProposedAction();
    }
}

void WgArchiveMaker::dropEvent(QDropEvent *event)
{
    const QMimeData *mime = event->mimeData();
    QList<QUrl>urls = mime->urls();
    if (mime->hasImage())
    {
        QImage img = mime->imageData().value<QImage>();
        QPixmap pixmap = QPixmap::fromImage(img);
        ui->label->setPixmap(pixmap);
    }

    QStringList errNameList;
    errNameList.clear();
    foreach (QUrl url, urls)
    {
        QString path = url.toLocalFile();
        QString name = QFileInfo(path).fileName();
        if (!m_nameList.contains(name))
        {
            QLabel *label = NULL;
            label = CreateImgLabel(path);
            if (label == NULL)
            {
                continue;
            }
            m_labelList.append(label);
            m_nameList.append(name);
            m_pathList.append(path);
        }
        else
        {
            errNameList << name;
        }
    }
    foreach (QLabel* label , m_labelList)
    {
        label->installEventFilter(this);
    }
    if (!errNameList.isEmpty())
    {
        QMessageBox::warning(this, TR("注意"), errNameList.join('\n') + TR("\n已存在"), QMessageBox::NoButton);
    }
    if (!m_labelList.isEmpty())
    {
        ui->label->setVisible(false);
    }
    Refresh();
}

void WgArchiveMaker::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);

    // 设置画笔为虚线
    QPen pen(Qt::black);
    pen.setStyle(Qt::DashDotLine);       // 设置为虚线
    pen.setWidth(1);
    painter.setPen(pen);

    QRect uiRect = ui->widget->rect();
    // 绘制虚线矩形
    uiRect.adjust(5, 5, 22, -5);
    painter.drawRect(uiRect);

    BaseWidget::paintEvent(event);
}

/**
 * @brief WgArchiveMaker::CreateImgLabel 创建图片组件
 * @param [in] path 文件或文件夹路径
 * @return 图像label指针
 */
QLabel *WgArchiveMaker::CreateImgLabel(const QString &path)
{
    if (path.isEmpty())
    {
        return NULL;
    }
    QString imagePath = "";
    QString fileName = "";
    bool isMore = false;
    if (path == "more")
    {
        imagePath = QString(":/img/more.png");
        fileName = TR("其他");
        isMore = true;
    }
    else
    {
        QFileInfo file(path);
        if (file.isDir())
        {
            imagePath = QString(":/img/folder.png");
        }
        else if (file.isFile())
        {
            imagePath = QString(":/img/file.png");
        }
        fileName = file.fileName();
    }
    QLabel *label = new QLabel(this);
    QString html = QString("<html><body><p>%1</p><br><img src='%2'></body></html>")
                       .arg(fileName, imagePath);
    label->setAlignment(Qt::AlignCenter);
    label->setText(html);
    label->setProperty("isMore", isMore);

    return label;
}

void WgArchiveMaker::on_selectDirBtn_clicked()
{
    QString path = QFileDialog::getExistingDirectory(this, "Select Folder", QCoreApplication::applicationDirPath());
    ui->lineEdit->setText(path);
    DataConfig::Instance().SetConfig("archive_cfg", "archive_savepath", path);
}

void WgArchiveMaker::on_clsBtn_clicked()
{
    ClearList();
    ui->label->setVisible(true);
    if (m_pMoreLabel != nullptr)
    {
        m_pMoreLabel->setVisible(false);
    }
    ui->label_top->setText("");
}

void WgArchiveMaker::on_startBtn_clicked()
{
    if (m_pathList.isEmpty())
    {
        QMessageBox::information(this, TR("提示"), TR("文件列表为空"), QMessageBox::NoButton, QMessageBox::NoButton);
        return;
    }

    QString nameStr = ui->lineEdit_2->text();
    if (nameStr.trimmed() == "")
    {
        QMessageBox::warning(this, TR("注意"), TR("文件名为空"), QMessageBox::NoButton);
        return;
    }
    QString dirPath = ui->lineEdit->text();
    DataConfig::Instance().SetConfig("archive_cfg", "archive_savefilename", nameStr);
    QString formatStr = ui->comboBox_format->currentText();
    if (dirPath.trimmed() != "")
    {
        nameStr = QString("%1/%2.%3").arg(dirPath, nameStr, formatStr);
    }
    else
    {
        nameStr = QString("%1/%2.%3").arg(QCoreApplication::applicationDirPath(), nameStr, formatStr); // 放到可执行文件目录下
    }
    if (QFile::exists(nameStr))
    {
        Bit32 res = QMessageBox::information(this, TR("提示"), TR("文件已存在是否覆盖"), QMessageBox::Yes, QMessageBox::No);
        if (res == QMessageBox::No)
        {
            return;
        }
        else
        {
            QFile::remove(nameStr);
        }
    }

    SevenZipExternalTool *sevenZipTool = dynamic_cast<SevenZipExternalTool *>(m_p7zTool.get());
    MD5ForBTFTool *md5Tool = dynamic_cast<MD5ForBTFTool *>(m_pMD5Tool.get());
    bool md5Required = false;
    if (sevenZipTool)
    {
        if (formatStr == "BTF")
        {
            formatStr = "tar";
            md5Required = true;
        }
        sevenZipTool->SetConfiguration(m_pathList, nameStr, formatStr);
        connect(sevenZipTool, &SevenZipExternalTool::ProgressValueChangedSignal, m_pDlgProcess, &DlgArchiveProcess::OnValueChanged);
        connect(sevenZipTool, &SevenZipExternalTool::MakeDoneSignal, m_pDlgProcess, &DlgArchiveProcess::Done);
        connect(sevenZipTool, &SevenZipExternalTool::MakeFaildSignal, m_pDlgProcess, &DlgArchiveProcess::Faild);
        if (md5Required)
        {
            if (md5Tool)
            {
                md5Tool->SetConfiguration(nameStr);
                connect(sevenZipTool->GetProcess(), QOverload<int, QProcess::ExitStatus>::of(&QProcess::finished),
                     md5Tool, &MD5ForBTFTool::OnReadyRunSlot, Qt::UniqueConnection);
            }
            else
            {
                QMessageBox::warning(this, TR("注意"), TR("创建md5工具失败"), QMessageBox::NoButton);
                TKLogger::Instance().AddLog(ERROR_LOG, TR("创建md5工具失败"));
            }
        }
        else
        {
            // 防止执行不必要的链接
            disconnect(sevenZipTool->GetProcess(), QOverload<int, QProcess::ExitStatus>::of(&QProcess::finished),
                     md5Tool, &MD5ForBTFTool::OnReadyRunSlot);
        }
        sevenZipTool->Run();
        m_pDlgProcess->ReSet();
        m_pDlgProcess->show();

    }
    else
    {
        QMessageBox::warning(this, TR("注意"), TR("创建7zip工具失败"),QMessageBox::NoButton);
        TKLogger::Instance().AddLog(ERROR_LOG, TR("创建7zip工具失败"));
    }
}

void WgArchiveMaker::OnFormatChangedSlot(const QString &arg1)
{
    DataConfig::Instance().SetConfig("archive_cfg", "archive_format", arg1);
}

void WgArchiveMaker::ClearList()
{
    foreach (QLabel *label, m_labelList)
    {
        if (label != nullptr)
        {
            ui->horizontalLayout->removeWidget(label);
            label->setParent(nullptr);
            delete label;
            label = nullptr;
        }
    }
    ui->horizontalLayout->removeWidget(m_pMoreLabel);
    m_labelList.clear();
    m_nameList.clear();
    m_pathList.clear();
}

void WgArchiveMaker::RemoveItem(QLabel *label)
{
    if (label == nullptr)
    {
        return;
    }

    Bit32 idx = m_labelList.indexOf(label);
    if (idx >= 0)
    {
        ui->horizontalLayout->removeWidget(label);
        m_labelList.removeAt(idx);
        m_nameList.removeAt(idx);
        m_pathList.removeAt(idx);
        label->removeEventFilter(this);
        label->setParent(NULL);
        delete label;
        label = NULL;
    }
    Bit32 totalCount = m_labelList.count();
    if (totalCount != 0)
    {
        ui->label_top->setText(TR("已选择%1个文件或文件夹").arg(m_labelList.count()));
    }
    else
    {
        ui->label_top->setText("");
    }

}

void WgArchiveMaker::Refresh()
{
    if (m_bHasMoreLabel)
    {
        ui->horizontalLayout->removeWidget(m_pMoreLabel);
        m_pMoreLabel->setVisible(false);
    }
    foreach (QLabel *label, m_labelList)
    {
        if (ContainsWidget(label))
        {
            continue;
        }
        else if (ui->horizontalLayout->count() == MAX_DISITEM_NUM)
        {
            if (m_pMoreLabel == nullptr)
            {
                m_pMoreLabel = CreateImgLabel("more");
                m_pMoreLabel->installEventFilter(this);
            }
            ui->horizontalLayout->addWidget(m_pMoreLabel);
            m_pMoreLabel->setVisible(true);
            m_bHasMoreLabel = true;
            break;
        }
        else if (label != nullptr)
        {
            ui->horizontalLayout->addWidget(label);
        }
    }
    Bit32 totalCount = m_labelList.count();
    if (totalCount != 0)
    {
        ui->label_top->setText(TR("已选择%1个文件或文件夹").arg(m_labelList.count()));
    }
}

bool WgArchiveMaker::ContainsWidget(QLabel *label)
{
    if (label == nullptr)
    {
        return false;
    }
    QWidget *targetWg = dynamic_cast<QWidget *>(label);
    for (Bit32 i = 0; i < ui->horizontalLayout->count(); i++)
    {
        QWidget *wg = ui->horizontalLayout->itemAt(i)->widget();
        if (wg == targetWg)
        {
            return true;
        }
    }

    return false;
}
