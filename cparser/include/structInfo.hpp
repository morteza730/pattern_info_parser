#pragma once

#include "iStructInfo.hpp"
#include <unordered_map>
#include <string>

namespace par
{
// TODO: a factory for making structInfo.
template <typename TAttribute>
struct StructInfo: public IStructInfo
{
    StructInfo(const std::string &path, std::unordered_map<TAttribute, std::string> attributeNames): path{path}, attributeNames{attributeNames}{}

    void fill(const NestedDictionary &data) override
    {
        for (auto it = attributeNames.cbegin(); it != attributeNames.cend(); ++it)
        {
            const std::string &targetKey = it->second;
            std::string result;

            std::function<bool(const NestedDictionary &)> recurse =
                [&](const NestedDictionary &node) -> bool
            {
                if (node.children.contains(targetKey) && !node.children[targetKey].value.empty() && node.children[targetKey].path == path)
                {
                    result = node.children[targetKey].value;
                    return true;
                }
                for (const auto &child : node.children)
                {
                    if (recurse(child))
                        return true;
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
