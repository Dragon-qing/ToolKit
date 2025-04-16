#ifndef HMIXMLEDITOR_H
#define HMIXMLEDITOR_H

#include <QObject>

class HmiXmlEditor
{
public:
    static HmiXmlEditor& Instance();
    HmiXmlEditor();
    ~HmiXmlEditor();

    // 设置xml文件
    void SetXmlFile(QString path);
private:

};

#endif // HMIXMLEDITOR_H
