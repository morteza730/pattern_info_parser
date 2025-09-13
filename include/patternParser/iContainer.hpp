#pragma once

#include <algorithm>
#include <map>
#include <vector>
#include <type_traits>

struct IContainer
{
    virtual ~IContainer() = default;
    virtual std::size_t count() = 0;
    std::string path;
};

template <typename TData>
struct MapContainer : public IContainer
{
    static_assert(std::is_default_constructible_v<TData>, "TData must be default constructible");

    using iterator = typename std::map<std::string, TData>::iterator;
    iterator begin() { return items.begin(); }
    iterator end() { return items.end(); }

    virtual std::size_t count() override final { return items.size(); }
    bool contain(const std::string &item) const { return items.find(item) != items.end(); }

    TData &operator[](const std::string &key)
    {
        if (!contain(key))
            items.emplace(key, TData());

        return items[key];
    }

private:
    std::map<std::string, TData> items;
};

template <typename TData>
struct ListContainer : public IContainer
{
    static_assert(std::is_default_constructible_v<TData>, "TData must be default constructible");

    using iterator = typename std::vector<TData>::iterator;
    iterator begin() { return items.begin(); }
    iterator end() { return items.end(); }

    virtual std::size_t count() override final { return items.size(); }
    int contain(const TData &data)
    {
        auto it = std::find(items.cbegin(), items.cend(), data);
        if (it != items.cend())
            return std::distance(items.cbegin(), it);
        else
            return -1;
    }

    std::size_t size() const { return items.size(); }
    void resize(int n) { items.resize(n); }
    TData &operator[](int index) { return items.at(index);}
    TData &at(int index) { return items.at(index); }

private:
    std::vector<TData> items;
};

template <typename TData>
struct LabelContainer : public IContainer
{
    static_assert(std::is_default_constructible_v<TData>, "TData must be default constructible");

    template <typename T, typename = void>
    struct has_equal_operator : std::false_type
    {
    };

    template <typename T>
    struct has_equal_operator<T, std::void_t<decltype(std::declval<T>() == std::declval<T>())>>
        : std::true_type
    {
    };

    static_assert(has_equal_operator<TData>::value, "TData should support ==");

    TData &value() {return item;}
    const TData &value() const {return item;}
    bool isEqual(const TData &data) { return item == data; }
    virtual std::size_t count() override final { return 1; }

private:
    TData item;

};
