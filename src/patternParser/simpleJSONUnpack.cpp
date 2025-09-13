#include "simpleJSONUnpack.hpp"
#include <regex>
#include <QJsonArray>

using namespace par;

bool tag_match_json(const Tag &tag, NestedData &result, const QJsonValue &jsonValue, std::string path);
bool tag_match_json_label(const par::Tag &tag, NestedData &result, const QJsonValue &jsonValue, std::string path);
bool tag_match_json_iterator(const par::Tag &tag, NestedData &result, const QJsonValue &jsonValue, std::string path);
bool tag_match_json_list(const par::Tag &tag, NestedData &result, const QJsonValue &jsonValue, std::string path);
bool tag_match_json_map(const par::Tag &tag, par::NestedData &result, const QJsonValue &jsonValue, std::string path);

bool tag_match_json_label(const par::Tag &tag, NestedData &result, const QJsonValue &jsonValue, std::string path)
{
    // auto *resultChild = dynamic_cast<LabelContainer<std::string> *>(result.children.get());
    result.tag = tag.name;
    result.value = jsonValue.toVariant().toString().toStdString();
    result.path = path;
    return true;
}

bool tag_match_json_iterator(const par::Tag &tag, NestedData &result, const QJsonValue &jsonValue, std::string path)
{
    path += '/' + tag.name;

    if (!jsonValue.isArray())
        return false;

    QJsonArray jsonArray = jsonValue.toArray();
    auto *resultChild = dynamic_cast<ListContainer<NestedData> *>(result.children.get());

    foreach (const QJsonValue &jsonValueChild, jsonArray)
    {
        resultChild->resize(resultChild->size() + 1);
        NestedData &resultElement = (*resultChild)[resultChild->size()-1];

        for (const Tag &tagChild : tag.children)
        {
            std::string index = std::to_string(resultChild->size()-1);

            if (tag_match_json(tagChild, resultElement, jsonValueChild, path + '/' + index))
                continue;
            else
                return false;
        }
    }
    return true;

}

bool tag_match_json_list(const par::Tag &tag, NestedData &result, const QJsonValue &jsonValue, std::string path)
{
    if (!jsonValue.isObject())
        return false;

    QJsonObject jsonObject = jsonValue.toObject();
    auto *resultChild = dynamic_cast<ListContainer<NestedData> *>(result.children.get());

    for (const Tag &tagChild : tag.children)
    {
        if (!jsonObject.contains(QString::fromStdString(tagChild.name)) && !tagChild.optional)
            return false;

        QJsonValue jsonValue = jsonObject.value(QString::fromStdString(tagChild.name));
        resultChild->resize(resultChild->size() + 1);
        NestedData &resultElement = (*resultChild)[resultChild->size()-1];

        if (tag_match_json(tagChild, resultElement, jsonValue, path))
            continue;
        else
            return false;
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

        QJsonValue jsonValue = jsonObject.value(QString::fromStdString(tagChild.name));
        NestedData &resultElement = (*resultChildMap)[tagChild.name];

        if (tag_match_json(tagChild, resultElement, jsonValue, path))
            continue;
        else
            return false;
    }
    return true;
}

bool tag_match_json(const Tag &tag, NestedData &result, const QJsonValue &jsonValue, std::string path)
{
    if (tag.type == Tag::Type::MAP)
    {
        if (!result.children)
            result.children = std::make_unique<MapContainer<NestedData>>();

        return tag_match_json_map(tag, result, jsonValue, path);
    }
    else if (tag.type == Tag::Type::ITERATOR)
    {
        if (!result.children)
            result.children = std::make_unique<ListContainer<NestedData>>();

        return tag_match_json_iterator(tag, result, jsonValue, path);
    }
    else if (tag.type == Tag::Type::LIST)
    {
        if (!result.children)
            result.children = std::make_unique<ListContainer<NestedData>>();

        return tag_match_json_list(tag, result, jsonValue, path);
    }
    else if (tag.type == Tag::Type::LABEL)
    {
        // if (!result.children)
        //     result.children = std::make_unique<LabelContainer<std::string>>();

        return tag_match_json_label(tag, result, jsonValue, path);
    }
    return false;
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
            QJsonValue jsonValue = jsonObject.value(QString::fromStdString(tagChild.name));
            if (tag_match_json(tagChild, resultElement, jsonValue, path))
                continue;
            else
                return false;
        }
        return true;
    }
    return false;
}
