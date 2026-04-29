#include <QRegularExpression>

#include "helpservice.h"

HelpService& HelpService::Instance()
{
    static HelpService instance;
    return instance;
}

HelpService::HelpService()
{

}

HelpService::~HelpService()
{

}

void HelpService::ParseHelpContent(const QString &rawContent)
{
    QRegularExpression re("^\\[(.+?)\\](.*)$");
    QRegularExpressionMatch match = re.match(rawContent);
    if (!match.hasMatch()) { // 格式不匹配，直接返回
        return;
    }

    QString category = match.captured(1).trimmed(); // 获取类别并去除前后空格
    QString content = match.captured(2).trimmed(); // 获取内容并去除前后空格
    if (category.isEmpty() || content.isEmpty()) { // 类别或内容为空，直接返回
        return;
    }

    if (m_hash.contains(category)) { // 已存在该类别，追加内容
        Bit32 index = m_hash.value(category);
        m_helpContentList[index].content.append(content);
    } else { // 新类别，创建新的HelpContentDTO
        HelpContentDTO helpContent;
        helpContent.category = category;
        helpContent.content.append(content);
        m_helpContentList.append(helpContent);
        m_hash.insert(category, m_helpContentList.size() - 1); // 存储类别和索引的映射
    }
}

void HelpService::ParseHelpContent(const QStringList &rawContentList)
{
    for (auto &rawContent : rawContentList) {
        ParseHelpContent(rawContent);
    }
}

QList<QString> HelpService::GetAllCategories() const
{
    return m_hash.keys();
}

QStringList HelpService::GetHelpContentByIndex(Bit32 index) const
{
    if (index < 0 || index >= m_helpContentList.size()) {
        return QStringList(); // 返回空的QStringList
    }
    
    return m_helpContentList.at(index).content;
}

void HelpService::Clear()
{
    m_helpContentList.clear();
    m_hash.clear();
}
