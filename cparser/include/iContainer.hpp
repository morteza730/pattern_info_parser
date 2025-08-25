#pragma once

#include <algorithm>
#include <map>
#include <vector>
#include <type_traits>

struct IContainer
{
    virtual ~IContainer() = default;
    virtual std::size_t count() = 0;
};

template <typename TNestedData>
struct MapContainer : public IContainer
{
    static_assert(std::is_default_constructible_v<TNestedData>, "TNestedData must be default constructible");

    std::map<std::string, TNestedData> items;

    using iterator = typename std::map<std::string, TNestedData>::iterator;
    iterator begin() { return items.begin(); }
    iterator end() { return items.end(); }

    virtual std::size_t count() override final { return items.size(); }
    bool contain(const std::string &item) const { return items.find(item) != items.end(); }

    TNestedData &operator[](const std::string &key)
    {
        if (!contain(key))
            items.emplace(key, TNestedData());

        return items[key];
    }
};

template <typename TNestedData>
struct ListContainer : public IContainer
{
    static_assert(std::is_default_constructible_v<TNestedData>, "TNestedData must be default constructible");

    std::vector<TNestedData> items;

    using iterator = typename std::vector<TNestedData>::iterator;
    iterator begin() { return items.begin(); }
    iterator end() { return items.end(); }

    virtual std::size_t count() override final { return items.size(); }
    bool contain(const TNestedData &data) { return std::find(items.begin(), items.end(), data) != items.end(); }
    std::size_t size() const { return items.size(); }
    void resize(int n) { items.resize(n); }
    TNestedData &at(int index) { return items.at(index); }
};

template <typename TNestedData>
struct LabelContainer : public IContainer
{
    static_assert(std::is_default_constructible_v<TNestedData>, "TNestedData must be default constructible");

    template <typename T, typename = void>
    struct has_equal_operator : std::false_type
    {
    };

    template <typename T>
    struct has_equal_operator<T, std::void_t<decltype(std::declval<T>() == std::declval<T>())>>
        : std::true_type
    {
    };

    static_assert(has_equal_operator<TNestedData>::value, "TNestedData should support ==");

    TNestedData item;

    bool isEqual(const TNestedData &data) { return item == data; }
};
