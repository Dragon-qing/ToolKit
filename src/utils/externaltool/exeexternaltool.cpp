/*
 * @Author: Dragon-qing
 * @Date: 2026-04-16
 * @LastEditors: Dragon-qing
 * @FilePath: \ToolKit\src\utils\externaltool\exeexternaltool.cpp
 * @Description: 可执行文件外部工具类，继承自ExternalToolBase，使用QProcess来管理外部工具的进程
 */
#include <QTimer>
#include <QFileInfo>
#include <QDir>
#include <QRegularExpression>

#include "common.h"
#include "tklogger.h"
#include "sysapi.h"

#include "spdlog/fmt/fmt.h"

#include "ExeExternalTool.h"

ExeExternalTool::ExeExternalTool(QObject *parent)
    : ExternalToolBase(parent)  
{
    Initialize();
}

ExeExternalTool::~ExeExternalTool()
{
    m_arguments.clear();
    if (m_process)
    {
        m_process->kill();
    }
}

void ExeExternalTool::Run()
{
    QString exeFileName = QFileInfo(m_sExePath).fileName();
    if (!m_process)
    {
        TKLogger::Instance().AddLog(ERROR_LOG, TR("%1进程未初始化").arg(exeFileName));
        return;
    }

    if (m_process->state() != QProcess::NotRunning)
    {
        TKLogger::Instance().AddLog(WARNING_LOG, TR("%1正在运行，无法启动").arg(exeFileName));
        return;
    }

    m_process->start(m_sExePath, m_arguments);
}

void ExeExternalTool::Stop()
{
    QString exeFileName = QFileInfo(m_sExePath).fileName();
    if (!m_process)
    {
        TKLogger::Instance().AddLog(ERROR_LOG, TR("%1进程未初始化").arg(exeFileName));
        return;
    }

    if (m_process->state() != QProcess::NotRunning)
    {
        m_process->terminate();

        // 超时强杀（非阻塞）
        QTimer::singleShot(3000, this, [this]() {
            if (m_process->state() != QProcess::NotRunning) {
                m_process->kill();
                TKLogger::Instance().AddLog(WARNING_LOG, TR("%1进程被强制终止").arg(QFileInfo(m_sExePath).fileName()));
            }
        });
    }
}

bool ExeExternalTool::IsRunning() const
{
    if (!m_process)
    {
        TKLogger::Instance().AddLog(ERROR_LOG, TR("外部exe工具进程未初始化"));
        return false;
    }

    if (m_process->state() != QProcess::NotRunning) 
    {
        return true;
    }
    return false;
}

QString ExeExternalTool::GetOutput()
{
    QByteArray data = m_process->readAll(); // 默认是读取标准输出
    return QString::fromLocal8Bit(data);
}

void ExeExternalTool::SetExecutablePath(const QString &path, bool checkValid)
{
    if (checkValid)
    {
        QFileInfo exeFileInfo(path);
        if (!exeFileInfo.exists() || !exeFileInfo.isExecutable())
        {
            TKLogger::Instance().AddLog(ERROR_LOG, TR("可执行文件路径:%1 无效").arg(path));
            return;
        }
    }

    m_sExePath = path;
}

void ExeExternalTool::SetArguments(const QStringList &args)
{
    m_arguments = args;
}

void ExeExternalTool::Initialize()
{
    m_arguments.clear();
    m_process.reset(new QProcess());
    m_sExePath = "";
    connect(m_process.data(), &QProcess::readyReadStandardOutput, this, &ExeExternalTool::OnReadyReadStandardOutput);
    connect(m_process.data(), &QProcess::readyReadStandardError, this, &ExeExternalTool::OnReadyReadStandardError);
    connect(m_process.data(), &QProcess::errorOccurred, this, &ExeExternalTool::OnErrorOccurred);
    connect(m_process.data(), QOverload<int, QProcess::ExitStatus>::of(&QProcess::finished), // 第二个参数是为了区分重载的finished信号,否则编译失败
         this, &ExeExternalTool::OnFinished);
}

QString ExeExternalTool::GetArgument(int index) const
{
    if (index < 0 || index >= m_arguments.size())
    {
        TKLogger::Instance().AddLog(DEBUG_LOG, TR("参数索引%1超出范围").arg(index));
        return QString();
    }

    return m_arguments.at(index);
}

void ExeExternalTool::OnErrorOccurred(QProcess::ProcessError error)
{
    QString exeFileName = QFileInfo(m_sExePath).fileName();
    switch (error)
    {
    case QProcess::FailedToStart:
        TKLogger::Instance().AddLog(ERROR_LOG, TR("%1启动失败").arg(exeFileName));
        break;
    case QProcess::Crashed:
        TKLogger::Instance().AddLog(ERROR_LOG, TR("%1崩溃").arg(exeFileName));
        break;
    case QProcess::Timedout:
        TKLogger::Instance().AddLog(ERROR_LOG, TR("%1超时").arg(exeFileName));
        break;
    case QProcess::ReadError:
        TKLogger::Instance().AddLog(ERROR_LOG, TR("%1读取错误").arg(exeFileName));
        break;
    case QProcess::WriteError:
        TKLogger::Instance().AddLog(ERROR_LOG, TR("%1写入错误").arg(exeFileName));
        break;
    case QProcess::UnknownError:
    default:
        TKLogger::Instance().AddLog(ERROR_LOG, TR("%1发生未知错误").arg(exeFileName));
        break;
    }
}

void ExeExternalTool::OnFinished(int exitCode, QProcess::ExitStatus status)
{
    QString exeFileName = QFileInfo(m_sExePath).fileName();
    if (status == QProcess::NormalExit && exitCode == 0) {
        TKLogger::Instance().AddLog(INFO_LOG, TR("%1进程已正常结束").arg(exeFileName));
    } else {
        TKLogger::Instance().AddLog(ERROR_LOG, TR("%1进程异常退出! ExitCode:%2").arg(exeFileName).arg(exitCode));
    }
}

// ------------------- SevenZipExternalTool ------------------
SevenZipExternalTool::SevenZipExternalTool(QObject *parent)
    : ExeExternalTool(parent)
{
    QString toolDirPath = GetSysPath(TOOLS_PATH); // 获取工具目录路径
    SetExecutablePath(QDir(toolDirPath).filePath("7z.exe"));
}

void SevenZipExternalTool::SetConfiguration(const QStringList &fileList, const QString &saveName, const QString &format)
{
    QStringList args;
    args << "a" << QString("-t%1").arg(format) << saveName << fileList << "-bsp1";
    SetArguments(args);
}

void SevenZipExternalTool::OnReadyReadStandardOutput()
{
    QString output = QString::fromLocal8Bit(GetProcess()->readAllStandardOutput());
    QRegularExpression re(R"((\d{1,3})%)");
    auto match = re.match(output);
    if (match.hasMatch()) {
        int percent = match.captured(1).toInt();
        emit ProgressValueChanged(percent);
    }
    if (output.contains("Everything is Ok")) {
        emit ProgressValueChanged(100);
        emit MakeDone();
        QString savePath = GetArgument(2);
        TKLogger::Instance().AddLog(INFO_LOG, TR("%1 打包完成").arg(savePath));
    }
}

void SevenZipExternalTool::OnReadyReadStandardError()
{
    TKLogger::Instance().AddLog(ERROR_LOG, TR("7zip error: %1").arg(QString::fromLocal8Bit(GetProcess()->readAllStandardError())));
    emit MakeFaild();
}

void SevenZipExternalTool::OnErrorOccurred(QProcess::ProcessError error)
{
    ExeExternalTool::OnErrorOccurred(error);
    emit MakeFaild();
}

// ------------------- MD5ForBTFTool ------------------
MD5ForBTFTool::MD5ForBTFTool(QObject *parent)
    : ExeExternalTool(parent)
{
}

void MD5ForBTFTool::OnReadyReadStandardOutput()
{
}

void MD5ForBTFTool::OnReadyReadStandardError()
{
}

REGISTER_TOOL("7zip", SevenZipExternalTool)
REGISTER_TOOL("md5forbtf", MD5ForBTFTool)
