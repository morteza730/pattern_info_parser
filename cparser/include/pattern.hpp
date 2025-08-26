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
        MAP,
        PATTERN
    } type;

protected:
    Tag(Type type, const std::string &name, bool optional)
        : type{type}, name{name}, optional{optional} {}

    Tag(Type type, const std::string &name, bool optional, const std::vector<Tag> &children)
        : type{type}, name{name}, optional{optional}, children{children} {}
};

class Map : public Tag
{
public:
    explicit Map(const std::string &name, bool optional, std::initializer_list<Tag> children)
        : Tag(Tag::Type::MAP, name, optional, children) {}
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
    explicit List(const std::string &name, bool optional, std::initializer_list<Tag> children)
        : Tag{Tag::Type::LIST, name, optional, children} {}
};

template <typename ...Args>
class Pattern: public Tag
{
public:
    explicit Pattern(Args&& ...args)
        : Tag{Tag::Type::PATTERN, "", false, std::vector<Tag>{std::forward<Args>(args)...}} {}
};
}
