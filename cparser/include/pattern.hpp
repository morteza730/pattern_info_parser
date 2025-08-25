#pragma once

#include <string>
#include <map>
#include <vector>
#include <sstream>
#include <iostream>

namespace par
{
class Tag
{
public:
    std::string name;
    bool optional;
    std::vector<Tag> children;

    friend std::ostream &operator<<(std::ostream &os, const Tag &tag)
    {
        os << "<" << tag.name;

        if (tag.children.size() == 0)
            os << "/>" << std::endl;
        else
        {
            os << ">" << std::endl;

            for (const auto &child : tag.children)
                os << child;

            os << "</" << tag.name << ">" << std::endl;
        }
        return os;
    }

    enum class Type
    {
        LABEL,
        LIST,
        PATTERN
    } type;

protected:
    Tag(Type type, const std::string &name, bool optional)
        : type{type}, name{name}, optional{optional} {}

    Tag(Type type, const std::string &name, bool optional, const std::vector<Tag> &children)
        : type{type}, name{name}, optional{optional}, children{children} {}
};

class Pattern : public Tag
{
public:
    explicit Pattern(const std::string &name, bool optional, std::initializer_list<Tag> children)
        : Tag(Tag::Type::PATTERN, name, optional, children) {}
};

class Label : public Tag
{
public:
    explicit Label(const std::string &name, bool optional = false)
        : Tag{Tag::Type::LABEL, name, optional} {}
};

class List : public Tag
{
public:
    explicit List(bool optional, std::initializer_list<Tag> children)
        : Tag{Tag::Type::LIST, "",optional, children} {}
};

}
