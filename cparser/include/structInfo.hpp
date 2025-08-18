#pragma once

#include "iStructInfo.hpp"
#include <unordered_map>
#include <string>
#include <functional>
#include <iostream>

namespace par
{
// TODO: a factory for making structInfo.
template <typename TAttribute>
struct StructInfo: public IStructInfo
{
    StructInfo(const std::string &path, std::unordered_map<TAttribute, std::string> attributeNames): path{path}, attributeNames{attributeNames}{}

    // TODO: fill function violate single responsibility and open close principles.
    void fill(const NestedData &data) override
    {
        for (auto it = attributeNames.cbegin(); it != attributeNames.cend(); ++it)
        {
            const std::string &targetKey = it->second;
            std::string result;

            std::function<bool(const NestedData &)> recurse =
                [&](const NestedData &node) -> bool
            {
                if (MapContainer<NestedData> *resultChildMap = dynamic_cast<MapContainer<NestedData> *>(node.children.get()))
                {
                    if (resultChildMap->contain(targetKey))
                        if (!(*resultChildMap)[targetKey].value.empty())
                            if ( (*resultChildMap)[targetKey].path == path)
                                {
                                    result = (*resultChildMap)[targetKey].value;
                                    return true;
                                }

                    for (auto it = resultChildMap->begin(); it != resultChildMap->end(); it++)
                    {
                        if (recurse(it->second))
                            return true;
                    }
                }
                else if (ListContainer<NestedData> *resultChildList = dynamic_cast<ListContainer<NestedData> *>(node.children.get()))
                {
                    for (auto it = resultChildList->begin(); it != resultChildList->end(); it++)
                    {
                        if (recurse(*it))
                            return true;
                    }
                }

                return false;
            };

            if (recurse(data))
            {
                attributeValues.emplace(it->first,result);
            }
        }
    }

    std::string get(TAttribute attribute) const
    {
        if (attributeValues.find(attribute) == attributeValues.end())
            return std::string();

        return attributeValues.at(attribute);
    }

    std::unordered_map<TAttribute, std::string> data() const {return attributeValues;}

    friend std::ostream &operator<<(std::ostream &os, const StructInfo<TAttribute> &obj)
    {
        for (auto it = obj.attributeNames.cbegin(); it != obj.attributeNames.cend(); ++it)
        {
            os << it->second << ": " << obj.get(it->first) << std::endl;
        }
        return os;
    }

private:
    std::unordered_map<TAttribute, std::string> attributeValues;
    std::unordered_map<TAttribute, std::string> attributeNames;
    std::string path;
};

}
