#include "simpleXMLUnpack.hpp"

bool par::tag_match_xml(const Tag &tag, NestedDictionary &result, const QDomElement &element, std::string path)
{
    QDomNodeList elementNodeList = element.childNodes();

    path += '/' + tag.name;

    for (const Tag &tagChild: tag.children)
    {
        for (int elementIndex=0; elementIndex< elementNodeList.length();)
        {
            QDomElement  elementChild = elementNodeList.at(elementIndex).toElement();
            if (tagChild.name == elementChild.nodeName().toStdString())
            {
                NestedDictionary &childResult = result.children[tagChild.name];
                childResult.value = elementChild.text().toStdString();
                childResult.path = path;

                if (tag_match_xml(tagChild,childResult,elementChild,path))
                    break;
                else
                    return false;
            }
            elementIndex++;

            if (elementIndex == elementNodeList.length() && !tagChild.optional)
                return false;
        }
    }
    return true;
}
