#pragma once

#include <functional>
#include <map>
#include "iContainer.hpp"

namespace par
{
template <typename TNestedData>
bool extract(const std::string &targetKey,
          const TNestedData &data,
          const std::function<bool(const TNestedData &)> &predicate,
          std::string &result)
{
    std::function<bool(const TNestedData &)> recurse =
        [&](const TNestedData &node) -> bool
    {
        if (MapContainer<TNestedData> *resultChildMap = dynamic_cast<MapContainer<TNestedData> *>(node.children.get()))
        {
            if (resultChildMap->contain(targetKey))
            {
                TNestedData &targetNode = (*resultChildMap)[targetKey];
                if (predicate(targetNode))
                {
                    result = (*resultChildMap)[targetKey].value;
                    return true;
                }
            }

            for (auto it = resultChildMap->begin(); it != resultChildMap->end(); it++)
            {
                if (recurse(it->second))
                    return true;
            }
        }
        else if (ListContainer<TNestedData> *resultChildList = dynamic_cast<ListContainer<TNestedData> *>(node.children.get()))
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
