#include "simpleJSONUnpack.hpp"
#include <regex>
#include <QJsonArray>

bool is_integer(const std::string &s)
{
    return std::regex_match(s, std::regex("^[-+]?\\d+$"));
}

bool tag_match_json_map(const par::Tag &tag, par::NestedData &result, const QJsonObject &jsonObject, std::string path){}

bool tag_match_json_vector(const par::Tag &tag, par::NestedData &result, const QJsonObject &jsonObject, std::string path){}

bool par::tag_match_json(const Tag &tag, NestedData &result, const QJsonObject &jsonObject, std::string path)
{
    QStringList objectList = jsonObject.keys();

    path += '/' + tag.name;

    for (const Tag &tagChild : tag.children)
    {

        if (!jsonObject.contains(QString::fromStdString(tagChild.name)) && !tagChild.optional)
        {
            return false;
        }

        // if (jsonChildValue.isArray())
        // {
        //     for (int index = 0; index < jsonChildValue.toArray().count(); index++)
        //     {
        //     }
        // }

        QJsonValue jsonChildValue = jsonObject.value(QString::fromStdString(tagChild.name));

        if (!jsonChildValue.isArray())
        {
            if (!result.children)
                result.children = std::make_unique<MapContainer<NestedData>>();

            NestedData &childResult = dynamic_cast<MapContainer<NestedData> *>(result.children.get())->getData(tagChild.name);

            childResult.value = jsonChildValue.toVariant().toString().toStdString();
            childResult.path = path;

            if (tag_match_json(tagChild, childResult, jsonChildValue.toObject(), path))
                continue;
            else
                return false;
        }
        else
        {
            if (!result.children)
                result.children = std::make_unique<ListContainer<NestedData>>();

            ListContainer<NestedData> *childList = dynamic_cast<ListContainer<NestedData> *>(result.children.get());
            
            if (tag_match_json(tagChild, childResult, jsonChildValue.toObject(), path))
                continue;
            else
                return false;
        }

        // Check if tag child refers to a a list.
        if (is_integer(tagChild.name))
        {
        }
    }
    return true;
}
