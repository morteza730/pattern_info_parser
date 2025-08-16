#pragma once

#include "iStructInfo.hpp"
#include <string>
#include <QMap>
#include <array>
#include <iostream>

namespace par
{
struct VasMenuInfo: IStructInfo
{
    enum class Attribute {
        MenuId,
        Name,
        Price,
        Description,
        TypeName,
        Code,
        ID,
        Counter,
        COUNT // Keep last for iteration
    };

    static const QMap<Attribute, std::string> attributeNames;

    VasMenuInfo(std::string path): path{path}
    {
        for (int i = 0; i < static_cast<int>(Attribute::COUNT); ++i) {
            attributes[static_cast<Attribute>(i)] = std::string();
        }
    }

    VasMenuInfo(std::initializer_list<std::pair<Attribute, std::string>> initList,std::string path = ".") : VasMenuInfo(path) {
        for (const auto& pair : initList) {
            attributes[pair.first] = pair.second;
        }
    }

    void fill(const NestedDictionary& data) override {
        for (auto it = attributeNames.constBegin(); it != attributeNames.constEnd(); ++it) {
            const std::string& targetKey = it.value();
            std::string result;

            std::function<bool(const NestedDictionary&)> recurse =
                [&](const NestedDictionary& node) -> bool {
                if (node.children.contains(targetKey) && !node.children[targetKey].value.empty() && node.children[targetKey].path == path) {
                    result = node.children[targetKey].value;
                    return true;
                }
                for (const auto& child : node.children) {
                    if (recurse(child)) return true;
                }
                return false;
            };

            if (recurse(data)) {
                attributes[it.key()] = result;
            }
        }
    }

    std::string get(Attribute attr) const { return attributes.value(attr, std::string()); }
    void set(Attribute attr, const std::string& value) { attributes[attr] = value; }

    friend std::ostream& operator<<(std::ostream& os, const VasMenuInfo& obj) {
        for (int i = 0; i < static_cast<int>(Attribute::COUNT); ++i) {
            auto attr = static_cast<Attribute>(i);
            os << attributeNames.value(attr)
               << ": " << obj.get(attr) << std::endl;
        }
        return os;
    }

private:
    QMap<Attribute, std::string> attributes;
    std::string path;
};

const QMap<VasMenuInfo::Attribute, std::string> VasMenuInfo::attributeNames = {
    {Attribute::MenuId, "MenuId"},
    {Attribute::Name, "Name"},
    {Attribute::Price, "Price"},
    {Attribute::Description, "Description"},
    {Attribute::TypeName, "TypeName"},
    {Attribute::Code, "Code"},
    {Attribute::ID, "ID"},
    {Attribute::Counter, "Counter"}
};

}
