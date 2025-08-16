#pragma once

#include "iStructInfo.hpp"
#include <string>
#include <array>
#include <iostream>
#include <unordered_map>

namespace par
{
struct RequestInfo : IStructInfo
{
    RequestInfo(std::string path, std::vector<std::string> requestedAttributes) : path{path}
    {
        for (const std::string& attribute: requestedAttributes)
        {
            attributes.emplace(attribute, std::string());
        }
    }

    void fill(const NestedDictionary &data) override
    {
        for (auto it = attributes.cbegin(); it != attributes.cend(); ++it)
        {
            const std::string &targetKey = it->first;
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
                attributes[targetKey] = result;
            }
        }
    }

    std::string get(const std::string &attribute) const
    {
        if (attributes.find(attribute) == attributes.end())
            return std::string();

        return attributes.at(attribute);
    }

    friend std::ostream &operator<<(std::ostream &os, const RequestInfo &obj)
    {
        for (const auto &it: obj.attributes)
        {
            os << it.first << ": " << it.second << std::endl;
        }
        return os;
    }

private:
    std::unordered_map<std::string, std::string> attributes;
    std::string path;
};

}
