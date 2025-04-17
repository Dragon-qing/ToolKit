#include <QFileInfo>
#include <QTextStream>
#include <QRegularExpression>

#include "common.h"

#include "hmixmleditor.h"

HmiXmlEditor &HmiXmlEditor::Instance()
{
    static HmiXmlEditor s_data;
    return s_data;
}

HmiXmlEditor::HmiXmlEditor() {}

HmiXmlEditor::~HmiXmlEditor()
{

}

void HmiXmlEditor::SetXmlFile(QString path, QStandardItemModel *model)
{
    if (model == NULL)
    {
        return;
    }
    QFileInfo xmlInfo(path);
    if (!xmlInfo.exists())
    {
        LogDt::Instance().AddLog(WARNING_LOG, QObject::TR("xmlEditor: %1文件不存在！").arg(path));
    }
    Bit32 ret = GetXmlDoc(path, m_docXml);
    if (ret < 0)
    {
        return;
    }
    QDomElement rootEle = m_docXml.documentElement(); // dom根节点
    TraverseElements(rootEle, model->invisibleRootItem());
}

QStringList HmiXmlEditor::GetAttrList(QStandardItemModel *model, const QModelIndex &index)
{
    QString context = model->data(index, ATTR_ROLE).toString();
    return context.split(" ");
}

/**
 * @brief HmiXmlEditor::SyncAttrList 同步model中的数据
 * @param treeModel
 * @param index
 * @param list
 */
void HmiXmlEditor::SyncAttrList(QStandardItemModel *treeModel, const QModelIndex &index, QStringList list)
{
    treeModel->setData(index, list.join(" "), ATTR_ROLE);
    QString displayText = treeModel->data(index).toString();
    QRegularExpression re("^(<\\w*\\s)(.*)(>)$");
    displayText.replace(re, QString("%1%2%3").arg("\\1").arg(list.join(" ")).arg("\\3"));
    treeModel->setData(index, displayText);
}

/**
 * @brief HmiXmlEditor::TraverseElements 递归遍历解析xml文件
 * @param element
 * @param modelItem
 */
void HmiXmlEditor::TraverseElements(const QDomElement &element, QStandardItem *modelItem)
{
    if (modelItem == NULL)
    {
        return;
    }

    QString content = QString("<%1").arg(element.tagName());
    QStringList attrList;
    attrList.clear();
    QDomNamedNodeMap attribuites = element.attributes();
    for (Bit32 i = 0; i < attribuites.count(); i++)
    {
        QDomAttr attr = attribuites.item(i).toAttr();
        content += QString(" %1=%2").arg(attr.name()).arg(attr.value());
        attrList.append(QString("%1=%2").arg(attr.name()).arg(attr.value()));
    }
    content += ">";
    QStandardItem *childItem = AddChildItemToModel(content, modelItem, attrList);
    QDomNode childNode = element.firstChild();
    while(!childNode.isNull())
    {
        if (childNode.isElement())
        {
            TraverseElements(childNode.toElement(), childItem);
        }
        childNode = childNode.nextSibling();
    }
}

/**
 * @brief HmiXmlEditor::AddChildItemToModel 想model中添加子项
 * @param name 子项名字
 * @param parent 父指针
 * @return 子指针
 */
QStandardItem* HmiXmlEditor::AddChildItemToModel(QString name, QStandardItem *parent, QStringList &attrList)
{
    if (parent == NULL)
    {
        return NULL;
    }
    QStandardItem *childItem = new QStandardItem(name);
    childItem->setData(attrList.join(" "), ATTR_ROLE);
    parent->appendRow(childItem);

    return childItem;
}
