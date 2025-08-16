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
        virtual ~INestedData() = 0;
        virtual std::size_t count() = 0;
    };

    struct IContainer
    {
        virtual ~IContainer() = 0;
        virtual std::size_t count() = 0;
        virtual bool contain(const std::string &item) = 0;
    };

    template <typename TNestedData, typename = std::enable_if_t<std::is_base_of_v<INestedData, TNestedData>>>
    struct MapContainer : public IContainer
    {
        std::map<std::string, TNestedData> items;

        virtual std::size_t count() override final { return items.size(); }
        virtual bool contain(const std::string &item) override final { return items.find(item) != items.end(); }

        TNestedData &getData(const std::string &key) { return items[key]; }
    };

    template <typename TNestedData, typename = std::enable_if_t<std::is_base_of_v<INestedData, TNestedData>>>
    struct ListContainer : public IContainer
    {
        std::vector<TNestedData> items;

        virtual std::size_t count() override final { return items.size(); }
        virtual bool contain(const std::string &item) override final { return std::find(items.begin(), items.end(), item) != items.end(); }
    };

    struct NestedData : public INestedData
    {
        std::string value;
        std::string path;

        std::unique_ptr<IContainer> children;

        virtual std::size_t count() override final { return children ? children->count() + 1 : 1; }
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
