#pragma once

#include <string>
#include <map>
#include <vector>
#include <type_traits>
#include <algorithm>
#include <memory>

namespace par
{

    struct INestedData
    {
        virtual ~INestedData() = default;
        virtual std::size_t count() = 0;
    };

    struct IContainer
    {
        virtual ~IContainer() = default;
        virtual std::size_t count() = 0;
    };

    template <typename TNestedData>
    struct MapContainer : public IContainer
    {
        static_assert(std::is_base_of_v<INestedData, TNestedData>, "TNestedData must derive from INestedData");
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
        static_assert(std::is_base_of_v<INestedData, TNestedData>, "TNestedData must derive from INestedData");
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

    struct NestedData : public INestedData
    {
        NestedData() = default;

        std::string value;
        std::string path;
        std::unique_ptr<IContainer> children;

        virtual std::size_t count() override final { return children ? children->count() + 1 : 1; }
        bool operator==(const NestedData &other) const
        {
            return value == other.value &&
                   path == other.path;
        }
    };

    // struct NestedDictionary
    // {
    //     std::string value;
    //     std::string path;
    //     QMap<std::string, NestedDictionary> children;

    //     std::size_t count() const
    //     {
    //         std::size_t total = 1;
    //         for (const auto &child : children)
    //             total += child.count();

    //         return total;
    //     }
    // };
}
