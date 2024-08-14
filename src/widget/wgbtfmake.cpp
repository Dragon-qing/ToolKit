#include <QDragEnterEvent>
#include <QMimeData>
#include <QDebug>
#include <QFileInfo>
#include <QMessageBox>
#include <QFileDialog>
#include <QProcess>

#include "common.h"
#include "hmiconfig.h"

#include "wgbtfmake.h"
#include "ui_wgbtfmake.h"

WgBTFMake::WgBTFMake(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::WgBTFMake)
{
    ui->setupUi(this);
    ui->label_tip->setText("");

    m_labelList.clear();
    m_nameList.clear();
    m_pathList.clear();
    m_timer = new QTimer(this);
    ui->lineEdit->setText(HMIConfig::Instance().GetConfig("btf_cfg", "btf_savepath"));
    ui->lineEdit_2->setText(HMIConfig::Instance().GetConfig("btf_cfg", "btf_savefilename"));

    connect(m_timer, SIGNAL(timeout()), this, SLOT(TimeoutHandler()));
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
                if (m_labelList.isEmpty())
                {
                    ui->label->setVisible(true);
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
        QString imagePath = GetImage(path);
        if (imagePath != "" && !m_nameList.contains(name))
        {
            QLabel *label = new QLabel(this);
            QString html = QString("<html><body><p>%1</p><br><img src='%2'></body></html>")
                           .arg(name, imagePath);
            label->setAlignment(Qt::AlignCenter);
            label->setText(html);
            m_labelList.append(label);
            m_nameList.append(name);
            m_pathList.append(path);
        }
        else if (m_nameList.contains(name))
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
    RefreshWidget();
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
    if (!m_pathList.isEmpty())
    {
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
        QProcess runLogProcess;
        runLogProcess.start("cmd");
        runLogProcess.waitForStarted(-1);
        QString _7zPath = QString("\"%1/7z.exe\"").arg(TOOLS_PATH);
        QString _md5ProgPath = QString("\"%1/md5ForBTF.exe\"").arg(TOOLS_PATH);
        QStringList fileStrList;
        fileStrList.clear();
        foreach (QString file, m_pathList)
        {
            fileStrList.append(QString("\"%1\"").arg(file));
        }
        nameStr = "\"" + nameStr + "\"";
        QString cmdStr = QString("%1 a -ttar %2 %3\n").arg(_7zPath).arg(nameStr).arg(fileStrList.join(" "));
        runLogProcess.write(cmdStr.toLocal8Bit().data());
        runLogProcess.write("exit\n");
        runLogProcess.waitForFinished(-1);
        QString res = runLogProcess.readAll();
        if (res.contains("Everything is Ok"))
        {
            SetTip(TR("已打包完成"), true);
            runLogProcess.start("cmd");
            runLogProcess.waitForStarted(-1);
            cmdStr = QString("%1 %2\n").arg(_md5ProgPath, nameStr);
            runLogProcess.write(cmdStr.toLocal8Bit().data());
            runLogProcess.write("exit\n");
            runLogProcess.waitForFinished(-1);
            LogDt::Instance().AddLog(INFO_LOG, TR("%1已完成BTF打包").arg(nameStr));
        }
        else
        {
            QMessageBox::critical(this, TR("错误"), TR("已打包失败!!"), QMessageBox::NoButton);
            LogDt::Instance().AddLog(CRITICAL_LOG, res);
        }
        runLogProcess.close();
    }
    else
    {
        QMessageBox::information(this, TR("提示"), TR("文件列表为空"), QMessageBox::NoButton, QMessageBox::NoButton);
    }

}

QString WgBTFMake::GetImage(const QString &path)
{
    QFileInfo file(path);
    if (file.isDir())
    {
        return QString(":/img/folder.png");
    }
    else if (file.isFile())
    {
        return QString(":/img/file.png");
    }

    return QString();
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

}

void WgBTFMake::RefreshWidget()
{
    foreach (QLabel *label, m_labelList)
    {
        if (label != NULL)
        {
            ui->horizontalLayout->addWidget(label);
        }
    }
}

void WgBTFMake::SetTip(const QString &tipStr, bool isAutoClear, Bit32 mes)
{
    ui->label_tip->setText(tipStr);
    if (isAutoClear)
    {
        m_timer->start(mes);
    }
}

void WgBTFMake::TimeoutHandler()
{
    if (m_timer->isActive())
    {
        m_timer->stop();
    }
    ui->label_tip->setText("");
}
