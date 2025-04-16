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

void HmiXmlEditor::SetXmlFile(QString path)
{

}
