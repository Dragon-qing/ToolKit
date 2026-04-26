#ifndef ARCHIVESERVICE_H
#define ARCHIVESERVICE_H
#include <memory>

#include <QObject>

#include "datadef.h"
#include "externaltoolbase.h"

class ArchiveService : public QObject
{
    Q_OBJECT
public:
    ArchiveService();
    ~ArchiveService();

    // 清理缓存数据，释放内存
    void ClearCache();

    // 获取7z工具实例指针
    ExternalToolBase *Get7zTool() { return m_p7zTool.get(); }
    // 获取md5ForBTF工具实例指针
    ExternalToolBase *GetMD5Tool() { return m_pMD5Tool.get(); }
    // 获取文件或文件夹的路径列表
    QStringList& GetPathList() { return m_pathList; }
    // 获取文件或文件夹的名称列表
    QStringList& GetNameList() { return m_nameList; }
    // 是否为空
    bool IsEmpty() const { return m_pathList.isEmpty(); }

    /**
     * @brief: 添加文件或文件夹到缓存中
     * @param {QString&} path: 文件或文件夹的路径
     * @return {Bit32} 成功添加返回0，失败返回-1（如路径不存在）
     */
    Bit32 AddFileOrDir(const QString& path);
    /**
     * @brief: 删除指定索引的文件或文件夹
     * @param {Bit32} index: 要删除的文件或文件夹的索引
     * @return {Bit32} 成功删除返回0，失败返回-1（如索引不存在）
     */
    Bit32 RemoveByIndex(Bit32 index);
    /**
     * @brief: 检查名称是否已存在于列表中
     * @param {QString&} name: 文件或文件夹的名称
     * @return {bool} true表示名称已存在，false表示名称不存在
     */
    bool ContainsName(const QString& name) const;

    /**
     * @brief: 设置运行参数
     * @param {QString&} savePath: 保存路径
     * @param {QString&} format: 压缩格式
     * @param {bool} md5Required: 是否需要生成MD5
     * @return {void}
     */
    void SetConfig(const QString& savePath, const QString& format, bool md5Required);
    // 启动压缩任务
    Bit32 Start();

private:
    QStringList m_nameList; // 存储文件或文件夹的名称列表
    QStringList m_pathList; // 存储文件或文件夹的路径列表
    std::unique_ptr<ExternalToolBase> m_p7zTool; // 7z工具实例指针
    std::unique_ptr<ExternalToolBase> m_pMD5Tool; // md5ForBTF工具实例指针
    QString m_sSavePath; // 保存路径
    QString m_sFormat; // 压缩格式
    bool m_bMD5Required; // 是否需要生成MD5
};

#endif