#include <QFileInfo>
#include <QTextStream>
#include <QRegularExpression>

#include "common.h"

#include "dataxmleditor.h"

DataXmlEditor &DataXmlEditor::Instance()
{
    static DataXmlEditor s_data;
    return s_data;
}

DataXmlEditor::DataXmlEditor()
{
    m_docXml.clear();
    m_mapChange.clear();
}

DataXmlEditor::~DataXmlEditor()
{

}

void DataXmlEditor::SetXmlFile(QString path, QStandardItemModel *model)
{
    if (model == NULL)
    {
        return;
    }
    QFileInfo xmlInfo(path);
    if (!xmlInfo.exists())
    {
        Logger::Instance().AddLog(WARNING_LOG, QObject::TR("xmlEditor: %1文件不存在！").arg(path));
    }
    Bit32 ret = GetXmlDoc(path, m_docXml);
    if (ret < 0)
    {
        return;
    }
    m_sPath = path;
    QDomElement rootEle = m_docXml.documentElement(); // dom根节点
    TraverseElements(rootEle, model->invisibleRootItem(), 0, 0, LOAD_TYEP);
}

QStringList DataXmlEditor::GetAttrList(QStandardItemModel *model, const QModelIndex &index)
{
    QString context = model->data(index, ATTR_ROLE).toString();
    return context.split(" ");
}

/**
 * @brief DataXmlEditor::SyncAttrList 同步model中的数据
 * @param treeModel
 * @param index
 * @param list
 */
void DataXmlEditor::SyncAttrList(QStandardItemModel *treeModel, const QModelIndex &index, QStringList list)
{
    treeModel->setData(index, list.join(" "), ATTR_ROLE);
    QString displayText = treeModel->data(index).toString();
    QRegularExpression re("^(<\\w*\\s)(.*)(>)$");
    displayText.replace(re, QString("%1%2%3").arg("\\1").arg(list.join(" ")).arg("\\3"));
    treeModel->setData(index, displayText);
    QString key = treeModel->data(index, Qt::ToolTipRole).toString();
    m_mapChange.insert(key, list.join(" "));
}

void DataXmlEditor::Save()
{
    if (m_sPath.isEmpty())
    {
        return;
    }
    QDomElement rootEle = m_docXml.documentElement();
    TraverseElements(rootEle, NULL, 0, 0, SAVE_TYPE);
    SaveXmlDoc(m_sPath, m_docXml);
    m_mapChange.clear();
}

QStandardItem * DataXmlEditor::ParseXML(Bit32 idx, const QDomElement &element, Bit32 deepth, QStandardItem *modelItem)
{
    QString content = QString("<%1").arg(element.tagName());
    QStringList attrList;
    attrList.clear();
    QPair<Bit32, Bit32> pos(deepth, idx);
    QDomNamedNodeMap attribuites = element.attributes();
    for (Bit32 i = 0; i < attribuites.count(); i++)
    {
        QDomAttr attr = attribuites.item(i).toAttr();
        content += QString(" %1=%2").arg(attr.name()).arg(attr.value());
        attrList.append(QString("%1=%2").arg(attr.name()).arg(attr.value()));
    }
    content += ">";
    QStandardItem *childItem = AddChildItemToModel(content, modelItem, attrList, pos);

    return childItem;
}

/**
 * @brief DataXmlEditor::SaveChange 保存修改的属性值到doc节点
 * @param key 位置
 * @param element 待修改的doc节点
 */
void DataXmlEditor::SaveChange(QString key, QDomElement &element)
{
    if (!m_mapChange.contains(key) || element.isComment())
    {
        return;
    }

    QStringList attrList = m_mapChange.value(key).split(' ');
    foreach(QString attr, attrList)
    {
        QStringList tmpList = attr.split('=');
        if (tmpList.count() < 2)
        {
            continue;
        }

        element.setAttribute(QString(tmpList.at(0)), QString(tmpList.at(1)));
    }
}

/**
 * @brief DataXmlEditor::TraverseElements 递归遍历解析xml文件
 * @param element
 * @param modelItem
 * @param deepth 递归深度
 * @param width 递归广度
 * @param opt 操作类型
 */
void DataXmlEditor::TraverseElements(QDomElement &element, QStandardItem *modelItem, Bit32 deepth, Bit32 width, _OPERATOR_TYPE opt)
{
    if ((modelItem == NULL && opt != SAVE_TYPE) || element.isComment())
    {
        return;
    }
    QStandardItem *childItem = NULL;
    if (opt == LOAD_TYEP)
    {
        childItem = ParseXML(width, element, deepth, modelItem);
    }
    else if (opt == SAVE_TYPE)
    {
        QString key = QString("%1,%2").arg(deepth).arg(width);

        SaveChange(key, element);
    }
    QDomNode childNode = element.firstChild();
    width = 0;
    while(!childNode.isNull())
    {
        if (childNode.isElement())
        {
            QDomElement childEle = childNode.toElement();
            TraverseElements(childEle, childItem, deepth + 1, width, opt);
        }
        childNode = childNode.nextSibling();
        width += 1;
    }
}

/**
 * @brief DataXmlEditor::AddChildItemToModel 想model中添加子项
 * @param name 子项名字
 * @param parent 父指针
 * @param pos 位置
 * @return 子指针
 */
QStandardItem* DataXmlEditor::AddChildItemToModel(QString name, QStandardItem *parent, QStringList &attrList, QPair<Bit32, Bit32> pos)
{
    if (parent == NULL)
    {
        return NULL;
    }
    QStandardItem *childItem = new QStandardItem(name);
    childItem->setData(attrList.join(" "), ATTR_ROLE);
    childItem->setData(QString("%1,%2").arg(pos.first).arg(pos.second), Qt::ToolTipRole);
    parent->appendRow(childItem);

    return childItem;
}
