#pragma once

#include <iostream>
#include <functional>
#include "pattern.hpp"
#include "iInterpreter.hpp"
#include <QJsonDocument>
#include <QJsonObject>
#include <string>

namespace par
{    
bool tag_match_json(const Tag &tag, NestedData   &result,  const QJsonObject &jsonObject, std::string path);
    
class SimpleJSONUnpack: public IInterpreter
{
    using TagMatchFunc = std::function<bool(const Tag&,NestedData &,const QJsonObject&,std::string)>;

public:
    SimpleJSONUnpack(const Pattern &p, TagMatchFunc func)
        : p{p}, matcherFunc{func}
    {}

    virtual bool unpack(const std::string& package, NestedData  &result) const override
    {
        if (package.empty())
            return false;

        QJsonDocument jsonDocument = QJsonDocument::fromJson(QByteArray::fromStdString(package));
        if(!jsonDocument.isObject())
            return false;

        QJsonObject jsonObject = jsonDocument.object();
        return matcherFunc(p,result,jsonObject,".");
    }

    const Pattern &get_pattern() const {return p;}
    const TagMatchFunc &get_matcher() const {return matcherFunc;}

private:
    Pattern p;
    TagMatchFunc matcherFunc;
};

}
