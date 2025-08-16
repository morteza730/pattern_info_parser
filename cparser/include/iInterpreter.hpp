#pragma once

#include <string>
#include "nestedData.hpp"

namespace par
{

struct IInterpreter
{
    virtual bool unpack(const std::string& package, NestedData &result) const = 0;
};

}
