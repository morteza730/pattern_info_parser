#include <iostream>
#include "patternParser/parserFactory.hpp"
#include "patternParser/structInfo.hpp"

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

    std::string cashDeskJson = R"json(
        {
            "TerminalCardID": 105632,
            "ServiceTypeId": 15,
            "Title": "فروشگاه دهدار",
            "PAN": "1",
            "TerminalID": "31317014",
            "MerchantID": "1",
            "MerchantName": "تست امید نت",
            "Description": "...",
            "IsTwoReceipts": "false",
            "IsStraightToVAsMenu": false,
            "TitleEN": "CashDesk",
            "ServiceType": {
                "Id": 15,
                "Title": "CASHDESK",
                "Description": "صندوقی"
            },
            "Details": [
                {
                    "TerminalCardID": 105632,
                    "Name": "فروشگاه دهدار",
                    "Price": 0,
                    "Description": "payid=1;title=نام",
                    "TypeName": "none",
                    "Code": "000007000772"
                }
            ],
            "RegionID": 168,
            "WebServiceUrl": null,
            "EnTerminalGroupTitle": "Buy",
            "FaTerminalGroupTitle": "خرید",
            "TerminalGroupDescription": ""
        }
     )json";


    Pattern terminalCardPattern
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

    StructInfo<VasMenuAttribute> structInfo{"./Details/0",attributeNames};

    std::unique_ptr<par::IParser> parser_json = par::create_parser(&terminalCardPattern, InterpretType::JSON);
    parser_json->parse({structInfo},cashDeskJson);

    std::cout << structInfo;

    return 0;
}
