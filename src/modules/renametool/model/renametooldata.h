/*
 * @Author: Dragon-qing
 * @Date: 2026-04-18
 * @LastEditors: Dragon-qing
 * @FilePath: \ToolKit\src\modules\renametool\model\renametooldata.h
 * @Description: 批量重命名工具数据结构定义
 */
#ifndef RENAMETOOLDATA_H
#define RENAMETOOLDATA_H
#include <QString>
#include <QStringList>

// 文件项数据结构
struct FileItemDTO {
    QString originalPath;   // 原始完整路径
    QString originalName;   // 原始文件名（含扩展名）
    QString baseName;       // 文件名（不含扩展名
    QString extension;      // 扩展名

    bool selected = true;   // 是否参与重命名
};

// 规则枚举类
enum class RenameRuleType {
    Replace,        // 替换
    Sequence,       // 序号
    Custom          // 自定义表达式
};

// 重命名规则数据结构
struct RenameRuleDTO {
    RenameRuleType type; // 规则类型

    // 通用字段
    bool enabled = true;

    // Replace
    QString findText;
    QString replaceText;
    bool useRegex = false;
    bool caseSensitive = false;

    // Sequence
    int startIndex = 1; // 开始序号
    int step = 1;       // 步长
    int padding = 2;    // 序号填充位数
    QString format;    // e.g. "%03d"

    // Custom
    QString expression; // 自定义规则（例如脚本 or 模板）
};

// 重命名任务数据结构
struct RenameTaskDTO {
    QVector<FileItemDTO> files;             // 文件列表
    QVector<RenameRuleDTO> rules;           // 规则

    QString targetDirectory;   // 目标目录（可选，默认为原目录）
    bool previewMode = true;   // 是否只是预览
    bool applyRulesToFullName = false; // true: 规则作用于完整文件名(含扩展名)
};

// 预览结果数据结构
struct RenamePreviewDTO {
    QString originalName;
    QString newName;

    bool valid = true;          // 是否有效（比如非法字符）
    QString errorMessage;       // 冲突 / 非法原因
};

// 结果数据结构
struct RenameResultDTO {
    QString originalPath;
    QString newPath;

    bool success;
    QString errorMessage;
};
#endif
