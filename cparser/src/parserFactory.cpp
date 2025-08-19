#include "parserFactory.hpp"
//#include "simpleXMLUnpack.hpp"
#include "simpleJSONUnpack.hpp"
#include "parser.hpp"
#include "pattern.hpp"
#include "nestedData.hpp"


std::unique_ptr<par::IParser> par::create_parser(par::Pattern* p, InterpretType type)
{
    if (!p) return std::unique_ptr<par::IParser>();

    switch (type) {
    case InterpretType::XML:
        // FIXME
        break;
        // return std::make_unique<Parser<SimpleXMLUnpack>>(Parser<SimpleXMLUnpack>(*p,tag_match_xml));
    case InterpretType::JSON:
        return std::make_unique<Parser<SimpleJSONUnpack<NestedData>>>(Parser<SimpleJSONUnpack<NestedData>>(*p,tag_match_json));
    }

    return std::unique_ptr<par::IParser>();
}
