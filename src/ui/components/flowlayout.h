/*
 * @Author: Dragon-qing
 * @Date: 2026-04-24
 * @LastEditors: Dragon-qing
 * @FilePath: \ToolKit\src\ui\components\flowlayout.h
 * @Description: 自定义流式布局，支持自动换行
 */
#ifndef FLOWLAYOUT_H
#define FLOWLAYOUT_H

#include <QLayout>

class FlowLayout : public QLayout
{
public:
    explicit FlowLayout(QWidget *parent = nullptr);
    ~FlowLayout();

    void addItem(QLayoutItem *item) override;
    QSize sizeHint() const override;
    void setGeometry(const QRect &rect) override;
    QLayoutItem *itemAt(int index) const override;
    QLayoutItem *takeAt(int index) override;
    int count() const override;

private:
    QList<QLayoutItem*> m_itemList;
};

#endif