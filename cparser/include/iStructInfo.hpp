#pragma once

#include "nestedData.hpp"

namespace par
{
struct IStructInfo
{
    virtual void fill(const NestedData  & data) = 0;
};
}
