/*
 * @Author: Dragon-qing
 * @Date: 2026-04-16
 * @LastEditors: Dragon-qing
 * @FilePath: \ToolKit\src\utils\externaltool\toolfactory.h
 * @Description: 外部工具工厂类，负责创建和管理外部工具实例
 */
#ifndef TOOLFACTORYBASE_H
#define TOOLFACTORYBASE_H
#include <QScopedPointer>
#include <QHash>
#include <QString>

#include <memory>

#include "externaltoolbase.h"

class ToolFactory
{
public:
    using Creator = std::function<std::unique_ptr<ExternalToolBase>()>;
    ~ToolFactory();

    static ToolFactory& Instance(); // 单例工厂
    
    /**
     * @brief: 注册构造器
     * @param {QString&} name: 类名
     * @param {Creator} creator: 构造器
     * @return {bool} 注册成功返回true，失败返回false（如已存在同名构造器）
     */
    bool RegisterCreator(const QString& name, Creator creator);

    /**
     * @brief: 创建工具实例
     * @param {QString&} name: 类名
     * @return {std::unique_ptr<ExternalToolBase>}
     */
    std::unique_ptr<ExternalToolBase> CreateTool(const QString& name);
private:
    ToolFactory() = default;
    QHash<QString, Creator> m_creators;
};

#endif