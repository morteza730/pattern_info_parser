#pragma once

#include <memory>
#include <string>
#include "iparser.hpp"
#include "pattern.hpp"

namespace par
{
enum class InterpretType
{
    XML,
    JSON
};

class Tag;
std::unique_ptr<IParser> create_parser(Tag *p, InterpretType type);
}
