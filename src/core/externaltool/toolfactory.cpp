#include <xutility>

#include "toolfactory.h"

ToolFactory::~ToolFactory()
{

}

ToolFactory &ToolFactory::Instance()
{
    static ToolFactory s_data;
    return s_data;
}

bool ToolFactory::RegisterCreator(const QString &name, Creator creator)
{
    if (m_creators.contains(name)) {
        return false; // 已存在同名构造器
    }
    m_creators[name] = std::move(creator); // 使用std::move避免不必要的复制
    return true;
}

std::unique_ptr<ExternalToolBase> ToolFactory::CreateTool(const QString &name)
{
    if (m_creators.contains(name)) {
        return m_creators[name](); // 存在则返回构造器创建的工具实例
    }

    return nullptr; // 不存在则返回空指针
}
