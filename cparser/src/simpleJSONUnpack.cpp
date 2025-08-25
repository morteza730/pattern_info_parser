#include "simpleJSONUnpack.hpp"
#include <regex>
#include <QJsonArray>

using namespace par;

bool is_integer(const std::string &s)
{
    return std::regex_match(s, std::regex("^[-+]?\\d+$"));
}

bool tag_match_json_label(const std::vector<Tag> &tags, NestedData &result, const QJsonArray &jsonArray, std::string path);
bool tag_match_json_list(const std::vector<Tag> &tags, NestedData &result, const QJsonArray &jsonArray, std::string path);
bool tag_match_json_map(const par::Tag &tag, par::NestedData &result, const QJsonObject &jsonObject, std::string path);

bool tag_match_json_label(const par::Tag &tag, NestedData &result, const QJsonArray &jsonArray, std::string path)
{
    return false;
}

bool tag_match_json_list(const std::vector<Tag> &tags, NestedData &result, const QJsonArray &jsonArray, std::string path)
{
    auto *resultChildList = dynamic_cast<ListContainer<NestedData> *>(result.children.get());
    assert(resultChildList && "result.children must be a MapContainer<NestedData>");

    for (const Tag &tagElement : tags)
    {
        if (!is_integer(tagElement.name))
            return false;

        int index = std::stoi(tagElement.name);

        if (jsonArray.size() <= index)
            return false;

//        path += '/' + tagElement.name;
        QJsonValue jsonElementValue = jsonArray.at(index);

        if (resultChildList->size() <= index)
            resultChildList->resize(index + 1);

        NestedData &resultElement = resultChildList->at(index);

        if (!jsonElementValue.isArray())
        {
            resultElement.value = jsonElementValue.toVariant().toString().toStdString();
            resultElement.path = path;

            if (!resultElement.children)
                resultElement.children = std::make_unique<MapContainer<NestedData>>();

            if (tag_match_json_map(tagElement, resultElement, jsonElementValue.toObject(), path))
                continue;
            else
                return false;
        }
        else
        {
            if (!resultElement.children)
                resultElement.children = std::make_unique<ListContainer<NestedData>>();

            if (tag_match_json_list(tagElement.children, resultElement, jsonElementValue.toArray(), path))
                continue;
            else
                return false;
        }
    }
    return true;
}

bool tag_match_json_map(const par::Tag &tag, par::NestedData &result, const QJsonObject &jsonObject, std::string path)
{
    auto *resultChildMap = dynamic_cast<MapContainer<NestedData> *>(result.children.get());
    assert(resultChildMap && "result.children must be a MapContainer<NestedData>");

    path += '/' + tag.name;

    for (const Tag &tagChild : tag.children)
    {

        if (!jsonObject.contains(QString::fromStdString(tagChild.name)) && !tagChild.optional)
            return false;

        QJsonValue jsonChildValue = jsonObject.value(QString::fromStdString(tagChild.name));
        if (!result.children)
            result.children = std::make_unique<MapContainer<NestedData>>();

        NestedData &resultElement = (*resultChildMap)[tagChild.name];

        if (!jsonChildValue.isArray())
        {
            resultElement.value = jsonChildValue.toVariant().toString().toStdString();
            resultElement.path = path;

            if (!resultElement.children)
                resultElement.children = std::make_unique<MapContainer<NestedData>>();

            if (tag_match_json_map(tagChild, resultElement, jsonChildValue.toObject(), path))
                continue;
            else
                return false;
        }
        else
        {
            if (!resultElement.children)
                resultElement.children = std::make_unique<ListContainer<NestedData>>();

            path += '/' + tagChild.name;
            if (tag_match_json_list(tagChild.children, resultElement, jsonChildValue.toArray(), path))
                continue;
            else
                return false;
        }
    }
    return true;
}

bool par::tag_match_json(const Tag &tag, NestedData &result, const QJsonObject &jsonObject, std::string path)
{
    if (tag.type == Tag::Type::PATTERN) 
    {
        if (!result.children)
            result.children = std::make_unique<MapContainer<NestedData>>();

        return tag_match_json_map(tag, result, jsonObject, path);
    }
    else if (tag.type == Tag::Type::LIST)
    {
        if (!result.children)
            result.children = std::make_unique<ListContainer<NestedData>>();

        return tag_match_json_list(tag, result, jsonObject, path);
    }
    else if (tag.type == Tag::Type::LABEL)
    {
        if (!result.children)
            result.children = std::make_unique<MapContainer<NestedData>>();

        return tag_match_json_label(tag, result, jsonObject, path);

    }
}
