#pragma once

#include "nestedData.hpp"

namespace par
{
struct IStructInfo
{
    virtual ~IStructInfo() = default;
    virtual void fill(const NestedData  & data) = 0;
};
}
