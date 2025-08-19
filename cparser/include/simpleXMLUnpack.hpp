#pragma once

#include <iostream>
#include <functional>
#include "pattern.hpp"
#include "iInterpreter.hpp"
#include <QDomDocument>
#include <string>

namespace par
{
bool tag_match_xml(const Tag &tag, NestedData   &result,  const QDomElement &element, std::string path);

template <typename TNestedData>
class SimpleXMLUnpack: public IInterpreter
{
    using TagMatchFunc = std::function<bool(const Tag&,TNestedData &,const QDomElement&, std::string)>;

public:
    SimpleXMLUnpack(const Pattern &p, TagMatchFunc func)
        : p{p}, matcherFunc{func}
    {}

    virtual bool unpack(const std::string& package, TNestedData  &result) const override
    {
        if (package.empty())
            return false;

        QByteArray xmlText = QByteArray::fromStdString(package);
        QDomDocument doc;
        if (!doc.setContent(xmlText))
            return false;

        QDomElement element = doc.documentElement();
        return matcherFunc(p,result,element,".");
    }

    const Pattern &get_pattern() const {return p;}
    const TagMatchFunc &get_matcher() const {return matcherFunc;}

private:
    Pattern p;
    TagMatchFunc matcherFunc;
};

}
