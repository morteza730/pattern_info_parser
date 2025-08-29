#pragma once

#include <functional>
#include <map>
#include "iContainer.hpp"

namespace par
{

template <typename TData>
bool extract(const TData &data,
             const std::function<bool(const TData &)> &predicate,
             std::string &result)
{
    std::function<bool(const TData &)> recurse =
        [&](const TData &node) -> bool
    {
        if ( predicate(node))
        {
            // FIXME: instead of getting value member data, the node itself should copy and returned.
            // But due to NestedData uncopyable nature, it is a hard task.
            result = node.value;
            return true;
        }

        if (MapContainer<TData> *resultChildMap = dynamic_cast<MapContainer<TData> *>(node.children.get()))
        {
            for (auto it = resultChildMap->begin(); it != resultChildMap->end(); it++)
            {
                if (recurse(it->second))
                    return true;
            }
        }
        else if (ListContainer<TData> *resultChildList = dynamic_cast<ListContainer<TData> *>(node.children.get()))
        {
            for (auto it = resultChildList->begin(); it != resultChildList->end(); it++)
            {
                if (recurse(*it))
                    return true;
            }
        }

        return false;
    };

    return recurse(data);
}
}
