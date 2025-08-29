#include <iostream>
#include "parserFactory.hpp"
#include "structInfo.hpp"

using namespace par;

int main(int argc, char *argv[])
{
    std::string sampleXML = R"xml(
    <pattern>
        <IsSuccess>true</IsSuccess>
        <ErrorMessage></ErrorMessage>
        <Data>
            <Title>title</Title>
            <PAN>pan</PAN>
            <TerminalID>terminalID</TerminalID>
            <MerchantID>merchantID</MerchantID>
            <ServiceType>
                <Id>id</Id>
                <Title>serviceTypeTitle</Title>
                <Description>serviceTypeDescription</Description>
            </ServiceType>
            <Details>
                <TerminalCardDetail>
                    <Name>Name</Name>
                    <Price>Price</Price>
                    <Description>Description</Description>
                    <TypeName>TypeName</TypeName>
                    <Code>Code</Code>
                    <ID>ID</ID>
                </TerminalCardDetail>
            </Details>
        </Data>
    </pattern>
    )xml";


    std::string sampleJSON = R"json(
    {
        "IsSuccess": true,
        "ErrorMessage": "",
        "Data": {
            "Details": [
                {
                    "TerminalCardID": 100009,
                    "Name": "بارکد",
                    "Price": 0,
                    "Description": "BARCODE",
                    "TypeName": "BARCODE",
                    "Code": "000013000570"
                },
                {
                    "TerminalCardID": 100009,
                    "Name": "کلینیک",
                    "Price": 1,
                    "Description": "CLINIC",
                    "TypeName": "بیمارستان امام حسین کرمانشاه",
                    "Code": "000013000571"
                }
            ]
        }
    }
    )json";


    Pattern terminalCardPattern
        {
            Label{"IsSuccess"},
            Label{"ErrorMessage"},
            Map{"Data",false,
                {
                    Iterator{"Details",
                    {
                        List
                        {
                            Label{"Name"},
                            Label{"Price"},
                            Label{"Description"},
                            Label{"TypeName"},
                            Label{"Code"},
                            Label{"TerminalCardID"}
                        }
                    }
                }
            }
        }

    };


    enum class VasMenuAttribute {
        MenuId,
        Name,
        Price,
        Description,
        TypeName,
        Code,
        ID,
        Counter
    };

    const std::unordered_map<VasMenuAttribute, std::string> attributeNames = {
      {VasMenuAttribute::MenuId, "MenuId"},
      {VasMenuAttribute::Name, "Name"},
      {VasMenuAttribute::Price, "Price"},
      {VasMenuAttribute::Description, "Description"},
      {VasMenuAttribute::TypeName, "TypeName"},
      {VasMenuAttribute::Code, "Code"},
      {VasMenuAttribute::ID, "TerminalCardID"},
      {VasMenuAttribute::Counter, "Counter"}
    };

    StructInfo<VasMenuAttribute> structInfo{"./Data/Details/0",attributeNames};

    std::unique_ptr<par::IParser> parser_json = par::create_parser(&terminalCardPattern, InterpretType::JSON);
    parser_json->parse({structInfo},sampleJSON);

    std::cout << structInfo;

    return 0;
}
