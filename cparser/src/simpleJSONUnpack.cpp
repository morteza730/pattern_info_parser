#include "simpleJSONUnpack.hpp"
#include <regex>
#include <QJsonArray>

using namespace par;

bool tag_match_json_label(const par::Tag &tag, NestedData &result, const QJsonValue &jsonValue, std::string path);
bool tag_match_json_list(const par::Tag &tag, NestedData &result, const QJsonValue &jsonValue, std::string path);
bool tag_match_json_map(const par::Tag &tag, par::NestedData &result, const QJsonValue &jsonValue, std::string path);

bool tag_match_json_label(const par::Tag &tag, NestedData &result, const QJsonValue &jsonValue, std::string path)
{
    result.value = jsonValue.toVariant().toString().toStdString();
    result.path = path;
    return true;
}

bool tag_match_json_list(const par::Tag &tag, NestedData &result, const QJsonValue &jsonValue, std::string path)
{
    path += '/' + tag.name;

    if (!jsonValue.isArray())
        return false;

    QJsonArray jsonArray = jsonValue.toArray();
    auto *resultChildList = dynamic_cast<ListContainer<NestedData> *>(result.children.get());

    for (const QJsonValue &jsonValueChild : jsonArray)
    {
        for (const Tag &tagChild : tag.children)
        {
            if (!jsonValueChild.isObject())
                return false;

            resultChildList->resize(resultChildList->size() + 1);

            NestedData &resultElement = (*resultChildList)[resultChildList->size()-1];
            if (tag_match_json(tagChild, resultElement, jsonValueChild.toObject(), path))
                continue;
            else
                return false;
        }
    }
    return true;
}

bool tag_match_json_map(const par::Tag &tag, par::NestedData &result, const QJsonValue &jsonValue, std::string path)
{
    path += '/' + tag.name;

    if (!jsonValue.isObject())
        return false;

    QJsonObject jsonObject = jsonValue.toObject();
    auto *resultChildMap = dynamic_cast<MapContainer<NestedData> *>(result.children.get());

    for (const Tag &tagChild : tag.children)
    {
        if (!jsonObject.contains(QString::fromStdString(tagChild.name)) && !tagChild.optional)
            return false;

        NestedData &resultElement = (*resultChildMap)[tagChild.name];
        if (tag_match_json(tagChild, resultElement, jsonObject, path))
            continue;
        else
            return false;
    }
    return true;
}

bool par::tag_match_json(const Tag &tag, NestedData &result, const QJsonObject &jsonObject, std::string path)
{
    if (tag.type == Tag::Type::PATTERN)
    {
        if (!result.children)
            result.children = std::make_unique<MapContainer<NestedData>>();

        auto *resultChildMap = dynamic_cast<MapContainer<NestedData> *>(result.children.get());

        for (const Tag &tagChild : tag.children)
        {
            if (!jsonObject.contains(QString::fromStdString(tagChild.name)) && !tagChild.optional)
                return false;

            NestedData &resultElement = (*resultChildMap)[tagChild.name];
            if (tag_match_json(tagChild, resultElement, jsonObject, path))
                continue;
            else
                return false;
        }
        return true;
    }
    else if (tag.type == Tag::Type::MAP)
    {
        if (!result.children)
            result.children = std::make_unique<MapContainer<NestedData>>();

        QJsonValue jsonValue = jsonObject.value(QString::fromStdString(tag.name));
        return tag_match_json_map(tag, result, jsonValue, path);
    }
    else if (tag.type == Tag::Type::LIST)
    {
        if (!result.children)
            result.children = std::make_unique<ListContainer<NestedData>>();

        QJsonValue jsonValue = jsonObject.value(QString::fromStdString(tag.name));
        return tag_match_json_list(tag, result, jsonValue, path);
    }
    else if (tag.type == Tag::Type::LABEL)
    {
        if (!result.children)
            result.children = std::make_unique<LabelContainer<NestedData>>();

        QJsonValue jsonValue = jsonObject.value(QString::fromStdString(tag.name));
        return tag_match_json_label(tag, result, jsonValue, path);
    }
    return false;
}
