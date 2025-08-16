#pragma once

#include "iStructInfo.hpp"
#include <memory>
#include <string>

namespace par
{
class IParser
{
public:
    virtual int parse(std::initializer_list<std::reference_wrapper<IStructInfo>> structInfos,const std::string &package) = 0;
    virtual ~IParser() = default;
};
}
