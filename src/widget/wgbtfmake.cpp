/*!
 * @brief BTF包制作页面
 * @file wgbtfmake.cpp
 * @author Dragon_qing
 * @date 2024/10/20
 */

#include <QDragEnterEvent>
#include <QMimeData>
#include <QDebug>
#include <QFileInfo>
#include <QMessageBox>
#include <QFileDialog>
#include <QProcess>
#include <QPainter>

#include "common.h"
#include "hmiconfig.h"
#include "logdt.h"

#include "wgbtfmake.h"
#include "ui_wgbtfmake.h"

WgBTFMake::WgBTFMake(QWidget *parent) :
    BaseWidget(parent),
    ui(new Ui::WgBTFMake)
{
    ui->setupUi(this);
    ui->label_tip->setText("");
    ui->label_top->setText("");

    m_labelList.clear();
    m_nameList.clear();
    m_pathList.clear();
    m_pTimer = new QTimer(this);
    ui->lineEdit->setText(HMIConfig::Instance().GetConfig("btf_cfg", "btf_savepath"));
    ui->lineEdit_2->setText(HMIConfig::Instance().GetConfig("btf_cfg", "btf_savefilename"));
    m_pDlgInfo = new DlgBTFMakeInfo(this);
    m_bHasMoreLabel = false;
    m_pMoreLabel = NULL;
    m_pDlgProcess = new DlgBtfProcess(this);

    connect(m_pTimer, SIGNAL(timeout()), this, SLOT(TimeoutHandler()));
    setAcceptDrops(true);
}

WgBTFMake::~WgBTFMake()
{
    delete ui;
}

bool WgBTFMake::eventFilter(QObject *obj, QEvent *event)
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
                    m_pDlgInfo->Exec();
                }
                return true;
            }
        }
    }

    return QObject::eventFilter(obj, event);
}

void WgBTFMake::dragEnterEvent(QDragEnterEvent *event)
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

void WgBTFMake::dropEvent(QDropEvent *event)
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

void WgBTFMake::paintEvent(QPaintEvent *event)
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
 * @brief WgBTFMake::CreateImgLabel 创建图片组件
 * @param [in] path 文件或文件夹路径
 * @return 图像label指针
 */
QLabel *WgBTFMake::CreateImgLabel(const QString &path)
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

void WgBTFMake::on_selectDirBtn_clicked()
{
    QString path = QFileDialog::getExistingDirectory(this, "Select Folder", QCoreApplication::applicationDirPath());
    ui->lineEdit->setText(path);
    HMIConfig::Instance().SetConfig("btf_cfg", "btf_savepath", path);
}

void WgBTFMake::on_clsBtn_clicked()
{
    ClearList();
    ui->label->setVisible(true);
}

void WgBTFMake::on_startBtn_clicked()
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
    HMIConfig::Instance().SetConfig("btf_cfg", "btf_savefilename", nameStr);
    if (dirPath.trimmed() != "")
    {
        nameStr = QString("%1/%2.BTF").arg(dirPath, nameStr);
    }
    else
    {
        nameStr = QString("%1/%2.BTF").arg(QCoreApplication::applicationDirPath(), nameStr);
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

    BTFProcessThread *thread = new BTFProcessThread(this);
    m_pThread = thread;
    thread->SetConfig(m_pathList, nameStr);
    connect(thread, &BTFProcessThread::finished, thread, &BTFProcessThread::deleteLater);
    connect(thread, &BTFProcessThread::MakeDone, m_pDlgProcess, &DlgBtfProcess::Done);
    connect(thread, &BTFProcessThread::MakeFaild, m_pDlgProcess, &DlgBtfProcess::Faild);
    thread->start();
    m_pDlgProcess->Start();
    m_pDlgProcess->show();
}

void WgBTFMake::ClearList()
{
    foreach (QLabel *label, m_labelList)
    {
        if (label != NULL)
        {
            ui->horizontalLayout->removeWidget(label);
            label->setParent(NULL);
            delete label;
            label = NULL;
        }
    }
    ui->horizontalLayout->removeWidget(m_pMoreLabel);
    m_pMoreLabel->setVisible(false);
    m_labelList.clear();
    m_nameList.clear();
    m_pathList.clear();
}

void WgBTFMake::RemoveItem(QLabel *label)
{
    if (label == NULL)
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

void WgBTFMake::Refresh()
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
            if (m_pMoreLabel == NULL)
            {
                m_pMoreLabel = CreateImgLabel("more");
                m_pMoreLabel->installEventFilter(this);
            }
            ui->horizontalLayout->addWidget(m_pMoreLabel);
            m_pMoreLabel->setVisible(true);
            m_bHasMoreLabel = true;
            break;
        }
        else if (label != NULL)
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

void WgBTFMake::SetTip(const QString &tipStr, bool isAutoClear, Bit32 mes)
{
    ui->label_tip->setText(tipStr);
    if (isAutoClear)
    {
        m_pTimer->start(mes);
    }
}

bool WgBTFMake::ContainsWidget(QLabel *label)
{
    if (label == NULL)
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

void WgBTFMake::TimeoutHandler()
{
    if (m_pTimer->isActive())
    {
        m_pTimer->stop();
    }
    ui->label_tip->setText("");
}

BTFProcessThread::BTFProcessThread(QWidget *parent)
    :QThread(parent)
{

}

BTFProcessThread::~BTFProcessThread()
{

}

/**
 * @brief BTFProcessThread::SetConfig 设置配置
 * @param fileList 文件列表
 * @param saveName 保存名称
 */
void BTFProcessThread::SetConfig(const QStringList &fileList, const QString &saveName)
{
    m_fileList = fileList;
    m_sSaveName = saveName;
}

void BTFProcessThread::run()
{
    if (m_sSaveName.isEmpty() || m_fileList.isEmpty())
    {
        LogDt::Instance().AddLog(CRITICAL_LOG, TR("打包失败，文件列表为空或保存名字为空"));
        return;
    }
    QProcess runLogProcess;
    runLogProcess.start("cmd");
    runLogProcess.waitForStarted(-1);
    QString _7zPath = QString("\"%1/7z.exe\"").arg(TOOLS_PATH);
    QString _md5ProgPath = QString("\"%1/md5ForBTF.exe\"").arg(TOOLS_PATH);
    QStringList fileStrList;
    fileStrList.clear();
    foreach (QString file, m_fileList)
    {
        fileStrList.append(QString("\"%1\"").arg(file));
    }
    m_sSaveName = "\"" + m_sSaveName + "\"";
    QString cmdStr = QString("%1 a -ttar %2 %3\n").arg(_7zPath).arg(m_sSaveName).arg(fileStrList.join(" "));
    runLogProcess.write(cmdStr.toLocal8Bit().data());
    runLogProcess.write("exit\n");
    runLogProcess.waitForFinished(-1);
    QString res = runLogProcess.readAll();
    if (res.contains("Everything is Ok"))
    {
        runLogProcess.start("cmd");
        runLogProcess.waitForStarted(-1);
        cmdStr = QString("%1 %2\n").arg(_md5ProgPath, m_sSaveName);
        runLogProcess.write(cmdStr.toLocal8Bit().data());
        runLogProcess.write("exit\n");
        runLogProcess.waitForFinished(-1);
        LogDt::Instance().AddLog(INFO_LOG, TR("%1已完成BTF打包").arg(m_sSaveName));
        emit MakeDone();
    }
    else
    {
        LogDt::Instance().AddLog(CRITICAL_LOG, res);
        emit MakeFaild();
    }
    runLogProcess.close();
}
