#ifndef DATAXMLEDITOR_H
#define DATAXMLEDITOR_H

#include <QObject>
#include <QStandardItemModel>
#include <QStringListModel>
#include <QDomDocument>
#include <QMap>

#include "datadef.h"

class DataXmlEditor
{
public:
    static DataXmlEditor& Instance();
    DataXmlEditor();
    ~DataXmlEditor();

    // 设置xml文件
    void SetXmlFile(QString path, QStandardItemModel *model);
    QStringList GetAttrList(QStandardItemModel *model, const QModelIndex &index);
    void SyncAttrList(QStandardItemModel *treeModel, const QModelIndex &index, QStringList list);
    // 保存对xml文件的修改
    void Save();
private:
    enum _TYPE{
        ATTR_ROLE = Qt::UserRole + 1,   // 标签属性
        POS_ROLE,                       // 位置属性(eg:1,2  深度,广度)

        TYPE_NUM
    };

    enum _OPERATOR_TYPE{
        LOAD_TYEP = 0,  // 读取
        SAVE_TYPE = 1,  // 保存
        OPER_NUM
    };
    QDomDocument m_docXml;
    QMap<QString, QString> m_mapChange; // 修改缓存
    QString m_sPath; // xml路径

    void TraverseElements(QDomElement &element, QStandardItem *modelItem, Bit32 deepth, Bit32 idx, _OPERATOR_TYPE opt);
    QStandardItem* AddChildItemToModel(QString name, QStandardItem *parent, QStringList &attrList, QPair<Bit32, Bit32> pos);
    QStandardItem* ParseXML(Bit32 idx, const QDomElement &element, Bit32 deepth, QStandardItem *modelItem);
    void SaveChange(QString key, QDomElement &element);
};

#endif // DATAXMLEDITOR_H
