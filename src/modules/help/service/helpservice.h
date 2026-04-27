#ifndef HELPSERVICE_H
#define HELPSERVICE_H
#include <QStringList>
#include <QHash>

#include "helpmodel.h"
#include "datadef.h"

class HelpService
{
public:
    static HelpService& Instance();
    ~HelpService();
    /**
     * @brief: 解析字符串获取帮助分类和内容
     * @note: 类别格式为[category]content -> [类别]内容
     * @param {QString} &rawContent: 原字符串
     * @return {void}
     */
    void ParseHelpContent(const QString &rawContent);
    void ParseHelpContent(const QStringList &rawContentList);

    QVector<HelpContentDTO> GetAllHelpContent() const { return m_helpContentList; }
    QList<QString> GetAllCategories() const;
    Bit32 GetSize() const { return m_helpContentList.size(); }
    QStringList GetHelpContentByIndex(Bit32 index) const;

    void Clear();
private:
    QVector<HelpContentDTO> m_helpContentList; // 存储解析后的帮助内容
    QHash<QString, Bit32> m_hash; // 存储类别和对应内容索引的哈希表，便于快速查找

    HelpService();
};

#endif