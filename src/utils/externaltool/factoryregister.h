/*
 * @Author: Dragon-qing
 * @Date: 2026-04-16
 * @LastEditors: Dragon-qing
 * @FilePath: \ToolKit\src\utils\externaltool\factoryregister.h
 * @Description: 工厂注册器头文件
 */
#ifndef FACTORYREGISTER_H
#define FACTORYREGISTER_H
#include "toolfactory.h"

// 注册工具构造器宏
#define REGISTER_TOOL(KEY, TYPE)                                \
    namespace {                                                 \
        const bool registered_##TYPE = []() {                   \
            return ToolFactory::Instance().RegisterCreator(     \
                KEY,                                            \
                []() -> std::unique_ptr<ExternalToolBase> {     \
                    return std::make_unique<TYPE>();            \
                }                                               \
            );                                                  \
        }();                                                    \
    }

#endif