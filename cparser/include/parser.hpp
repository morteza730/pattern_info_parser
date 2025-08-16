#pragma once

#include "iStructInfo.hpp"
#include "iinterpreter.hpp"
#include "iparser.hpp"

namespace par
{
enum class InterpretType;
class Pattern;

template <typename T>
class Parser: public IParser,public T
{
    friend std::unique_ptr<IParser> create_parser(Pattern* p, InterpretType type);

public:
    static_assert(std::is_base_of<IInterpreter,T>::value,
                  "Template argument must be an interpretor."
                  );

    int parse(std::initializer_list<std::reference_wrapper<IStructInfo>> structInfos,const std::string &package) override
    {
        NestedData  result;
        if (!T::unpack(package,result))
            return -1;

        for (auto& structInfo : structInfos) {
            structInfo.get().fill(result);
        }

        return result.count();
    }

private:
    template<typename ...Args>
    Parser(Args&& ...args)
        : T{std::forward<Args>(args)...}
    {}
};

}
