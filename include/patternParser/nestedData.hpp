#pragma once

#include <string>
#include <map>
#include <vector>
#include <type_traits>
#include <algorithm>
#include <memory>
#include "iContainer.hpp"

namespace par
{

    struct INestedData
    {
        virtual ~INestedData() = default;
        virtual std::size_t count() = 0;
    };

    struct NestedData : public INestedData
    {
        NestedData() = default;

        std::string value;
        std::string path;
        std::string tag;
        std::unique_ptr<IContainer> children;

        virtual std::size_t count() override final { return children ? children->count() + 1 : 1; }
        bool operator==(const NestedData &other) const
        {
            return value == other.value &&
                   children->count() == other.children->count() &&
                   path == other.path;
        }
    };
}
