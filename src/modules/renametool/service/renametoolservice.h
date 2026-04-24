/*
 * @Author: Dragon-qing
 * @Date: 2026-04-18
 * @LastEditors: Dragon-qing
 * @FilePath: \ToolKit\src\modules\renametool\service\renametoolservice.h
 * @Description: 重命名工具服务类
 */
#ifndef RENAMETOOLSERVICE_H
#define RENAMETOOLSERVICE_H

#include <QObject>
#include <QVector>

#include "datadef.h"
#include "renametooldata.h"

class RenameToolService : public QObject
{
    Q_OBJECT
public:
    explicit RenameToolService(QObject *parent = nullptr);
    ~RenameToolService();

    /**
     * @brief 设置当前任务数据
     * @param {RenameTaskDTO} &task: 任务数据
     * @return {}
     */
    void SetTask(const RenameTaskDTO &task);
    RenameTaskDTO GetTask() const; // 获取当前任务数据

    void Run(); // 执行重命名任务

    bool IsRunning() const; // 是否正在执行重命名任务
    QString GetLastError() const; // 获取最近一次错误信息

signals:
    // 预览结果准备就绪信号
    void PreviewReadySignal(const QVector<RenamePreviewDTO> &previewList); 
    // 重命名完成信号
    void RenameFinishedSignal(const QVector<RenameResultDTO> &resultList); 
    // 错误信号
    void ErrorSignal(const QString &errorMessage); 
    // 进度更新信号
    void ProgressValueChangedSignal(Bit32 current, Bit32 total); 

private:
    struct PreviewItem {
        Bit32 fileIndex = -1;
        QString sourcePath;
        QString targetPath;
        RenamePreviewDTO preview;
    };

    RenameTaskDTO m_task; // 当前任务数据
    QString m_lastError; // 最近一次错误信息
    bool m_running; // 是否正在执行重命名任务

    // 异步预览重命名结果
    void PreviewAsync(); 
    // 异步执行重命名任务
    void ExecuteAsync(); 

    /**
     * @brief: 构建预览项列表
     * @return {QVector<PreviewItem>} 预览项列表
     */
    QVector<PreviewItem> BuildPreviewItems();
    /**
     * @brief: 应用规则生成新文件名
     * @param {FileItemDTO} &file: 文件项数据
     * @param {Bit32} sequenceIndex: 序号索引
     * @param {QString} &errorMessage: 错误信息
     * @return {QString} 新文件名
     */
    QString ApplyRulesToFileName(const FileItemDTO &file, Bit32 sequenceIndex, QString &errorMessage) const;

    /**
     * @brief: 验证生成的文件名是否合法
     * @param {QString} &fileName: 文件名
     * @param {QString} &errorMessage: 错误信息
     * @return {bool} 是否合法
     */
    bool ValidateFileName(const QString &fileName, QString &errorMessage) const;
    /**
     * @brief: 构建序号文本
     * @param {RenameRuleDTO} &rule: 重命名规则
     * @param {Bit32} sequenceValue: 序号值
     * @return {QString} 序号文本
     */
    QString BuildSequenceText(const RenameRuleDTO &rule, Bit32 sequenceValue) const;
    /**
     * @brief: 解析规则字符串中的预留字段
     * @note: 预留字段包括但不限于 {name}（原文件名不含扩展名），{ext}（扩展名），{seq}（序号）等
     * @param {QString} &text: 规则文本
     * @param {FileItemDTO} &file: 文件项数据
     * @param {bool} forRegexPattern: 是否用于正则表达式模式
     * @return {QString} 解析后的文本
     */
    QString ResolveReservedFields(const QString &text, const FileItemDTO &file,
                                  bool forRegexPattern) const;
    /**
     * @brief: 设置错误信息
     * @param {QString} &errorMessage: 错误信息
     * @return {void}
     */
    void SetError(const QString &errorMessage);
};

#endif
