/*
 * @Author: Dragon-qing
 * @Date: 2026-04-16
 * @LastEditors: Dragon-qing
 * @FilePath: \ToolKit\src\utils\externaltool\exeexternaltool.h
 * @Description: 可执行文件外部工具类，继承自ExternalToolBase，使用QProcess来管理外部工具的进程(异步)
 */
#ifndef EXEEXTERNALTOOL_H
#define EXEEXTERNALTOOL_H
#include <QScopedPointer>
#include <QProcess>

#include "factoryregister.h"

#include "externaltoolbase.h"

class ExeExternalTool : public ExternalToolBase
{
    Q_OBJECT
public:
    explicit ExeExternalTool(QObject *parent = nullptr);
    ~ExeExternalTool();
    
    void Run() override; 
    void Stop() override;
    bool IsRunning() const override;
    QString GetOutput() override;
    
    QProcess *GetProcess() const { return m_process.data(); } // 获取QProcess对象指针，供外部使用
    
protected:
    /**
     * @brief: 内部使用的设置可执行文件路径函数，增加了是否检查路径有效性的参数
     * @param {QString} &path: 文件路径
     * @param {bool} checkValid: 是否检查路径有效性，默认为true
     * @return {}
     */
    void SetExecutablePath(const QString &path, bool checkValid = true);
    void SetArguments(const QStringList &args) override;

protected slots:
    virtual void OnReadyReadStandardOutput() = 0; // 读取标准输出的槽函数
    virtual void OnReadyReadStandardError() = 0; // 读取标准错误的槽函数
    
private:
    QStringList m_arguments; // 存储工具的参数
    QScopedPointer<QProcess> m_process; // 用于管理外部工具的进程
    QString m_sExePath; // 可执行文件路径
    
    void Initialize();

private slots:
    void OnErrorOccurred(QProcess::ProcessError error);
    void OnFinished(int exitCode, QProcess::ExitStatus status);
};

class SevenZipExternalTool : public ExeExternalTool
{
    Q_OBJECT
public:
    explicit SevenZipExternalTool(QObject *parent = nullptr);
    ~SevenZipExternalTool() = default;
    void SetConfiguration(const QStringList &fileList, const QString &saveName, const QString &format = "tar");

private slots:

    void OnReadyReadStandardOutput() override; // 读取标准输出的槽函数
    void OnReadyReadStandardError() override; // 读取标准错误的槽函数
};

class MD5ForBTFTool : public ExeExternalTool
{
    Q_OBJECT    
public:
    explicit MD5ForBTFTool(QObject *parent = nullptr);
    ~MD5ForBTFTool() = default;

private slots:
    void OnReadyReadStandardOutput() override; // 读取标准输出的槽函数
    void OnReadyReadStandardError() override; // 读取标准错误的槽函数
};
#endif
