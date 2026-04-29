#include "uifactory.h"

UIFactory& UIFactory::Instance()
{
    static UIFactory instance;
    return instance;
}

UIFactory::UIFactory()
{

}

UIFactory::~UIFactory()
{

}

bool UIFactory::RegisterPage(const QString& key, UIMenuType menu, const QString& text, PageCreator c, Bit32 order)
{
    const QString normalizedKey = key.trimmed();
    const QString normalizedText = text.trimmed();

    if (normalizedKey.isEmpty() || normalizedText.isEmpty() || !c)
    {
        return false;
    }

    // key 在页面和对话框注册表中都必须全局唯一
    if (m_pageCreators.contains(normalizedKey) || m_dialogCreators.contains(normalizedKey))
    {
        return false;
    }

    m_pageCreators.insert(normalizedKey, std::move(c));
    m_entries.append(UIEntry{UIItemType::Page, menu, normalizedKey, normalizedText, order});
    return true;
}

bool UIFactory::RegisterDialog(const QString& key, UIMenuType menu, const QString& text, DialogCreator c, Bit32 order)
{
    const QString normalizedKey = key.trimmed();
    const QString normalizedText = text.trimmed();

    if (normalizedKey.isEmpty() || normalizedText.isEmpty() || !c)
    {
        return false;
    }

    // key 在页面和对话框注册表中都必须全局唯一
    if (m_pageCreators.contains(normalizedKey) || m_dialogCreators.contains(normalizedKey))
    {
        return false;
    }

    m_dialogCreators.insert(normalizedKey, std::move(c));
    m_entries.append(UIEntry{UIItemType::Dialog, menu, normalizedKey, normalizedText, order});
    return true;
}

QWidget* UIFactory::CreatePage(const QString& key, QWidget* parent) const
{
    const QString normalizedKey = key.trimmed();
    const auto it = m_pageCreators.constFind(normalizedKey);
    if (it == m_pageCreators.constEnd())
    {
        return nullptr;
    }

    return it.value()(parent);
}

QDialog* UIFactory::CreateDialog(const QString& key, QWidget* parent) const
{
    const QString normalizedKey = key.trimmed();
    const auto it = m_dialogCreators.constFind(normalizedKey);
    if (it == m_dialogCreators.constEnd())
    {
        return nullptr;
    }

    return it.value()(parent);
}

const QList<UIEntry>& UIFactory::Entries() const
{
    return m_entries;
}
