#include "flowlayout.h"

FlowLayout::FlowLayout(QWidget *parent)
    : QLayout(parent)
{
}

FlowLayout::~FlowLayout()
{

}

void FlowLayout::addItem(QLayoutItem *item)
{
    m_itemList.append(item);
}

QSize FlowLayout::sizeHint() const 
{
    QSize size;
    for (auto item : m_itemList)
        size = size.expandedTo(item->sizeHint()); // 取所有子控件的最大尺寸作为参考

    return size;
}

void FlowLayout::setGeometry(const QRect &rect)
{
    QLayout::setGeometry(rect);

    int x = rect.x();
    int y = rect.y();
    int lineHeight = 0;

    for (QLayoutItem *item : m_itemList)
    {
        int spaceX = spacing();
        int spaceY = spacing();

        int nextX = x + item->sizeHint().width() + spaceX;

        // 换行判断
        if (nextX - spaceX > rect.right() && lineHeight > 0)
        {
            x = rect.x();
            y += lineHeight + spaceY;
            nextX = x + item->sizeHint().width() + spaceX;
            lineHeight = 0;
        }

        item->setGeometry(QRect(QPoint(x, y), item->sizeHint()));

        x = nextX;
        lineHeight = std::max(lineHeight, item->sizeHint().height());
    }
}

QLayoutItem *FlowLayout::itemAt(int index) const
{
    return m_itemList.value(index);
}

QLayoutItem *FlowLayout::takeAt(int index)
{
    if (index < 0 || index >= m_itemList.count()) {
        return nullptr;
    }

    return m_itemList.takeAt(index);
}

int FlowLayout::count() const
{
    return m_itemList.count();
}