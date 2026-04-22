/*
 * @Author: Dragon-qing
 * @Date: 2026-04-16
 * @LastEditors: Dragon-qing
 * @FilePath: \ToolKit\src\core\externaltool\externaltoolbase.h
 * @Description: 外部工具基类，定义了外部工具的基本接口和功能
 */
#ifndef EXTERNALTOOLBASE_H
#define EXTERNALTOOLBASE_H
#include <QStringList>
#include <QObject>

class ExternalToolBase : public QObject
{
    Q_OBJECT
public:
    explicit ExternalToolBase(QObject *parent = nullptr);
    ~ExternalToolBase() = default;

    /**
     * @brief: 运行工具
     * @return {}
     */
    virtual void Run() = 0;

    /**
     * @brief: 停止工具
     * @return {}
     */    
    virtual void Stop() = 0;

    /**
     * @brief: 检查工具是否正在运行
     * @return {bool} true表示正在运行，false表示未运行
     */
    virtual bool IsRunning() const = 0;

    /**
     * @brief: 获取工具的输出结果
     * @return {QString} 工具的输出结果
     */
    virtual QString GetOutput() = 0;

protected:
        /**
     * @brief: 设置参数
     * @param {QStringList} &args: 参数列表
     * @return {}
     */
    virtual void SetArguments(const QStringList &args) = 0;
};

#endif // EXTERNALTOOLBASE_H
