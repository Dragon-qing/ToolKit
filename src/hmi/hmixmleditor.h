#ifndef HMIXMLEDITOR_H
#define HMIXMLEDITOR_H

#include <QObject>
#include <QStandardItemModel>
#include <QStringListModel>
#include <QDomDocument>

class HmiXmlEditor
{
public:
    static HmiXmlEditor& Instance();
    HmiXmlEditor();
    ~HmiXmlEditor();

    // 设置xml文件
    void SetXmlFile(QString path, QStandardItemModel *model);
    QStringList GetAttrList(QStandardItemModel *model, const QModelIndex &index);
    void SyncAttrList(QStandardItemModel *treeModel, const QModelIndex &index, QStringList list);
private:
    enum _TYPE{
        ATTR_ROLE = Qt::UserRole + 1, // 标签属性

        TYPE_NUM
    };

    QDomDocument m_docXml;

    void TraverseElements(const QDomElement &element, QStandardItem *modelItem);
    QStandardItem* AddChildItemToModel(QString name, QStandardItem *parent, QStringList &attrList);

};

#endif // HMIXMLEDITOR_H
