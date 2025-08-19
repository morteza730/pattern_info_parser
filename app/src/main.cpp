#include <iostream>
#include "parserFactory.hpp"
//#include "transactionInfo.hpp"
// #include "requestInfo.hpp"
//#include "vasMenuInfo.hpp"
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

    std::string salamatJSON = R"json(
    {
    "IsSuccess": true,
        "ErrorMessage": "",
        "Data":{
            "TerminalCardID": 100009,
            "ServiceTypeId": 12,
            "Title": "emam hossein hospital",
            "PAN": "1",
            "TerminalID": "212857",
            "MerchantID": "21061182",
            "MerchantName": "emam hossein",
            "Description": "user=Cashless_3;password=123;url=http://192.168.1.114:8080/samensalamat/api/cashier",
            "IsTwoReceipts": false,
            "IsStraightToVAsMenu": true,
            "TitleEN": "SamenSalamatHis",
            "ServiceType": {
                "Id": 12,
                "Title": "HOSPITAL_S",
                "Description": "بیمارستان ثامن سلامت"
            },
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
                },
                {
                "TerminalCardID": 100009,
                "Name": "دندانپزشکی",
                "Price": 2,
                "Description": "DENTIST",
                "TypeName": "بیمارستان امام حسین کرمانشاه",
                "Code": "000013000572"
                },
                {
                "TerminalCardID": 100009,
                "Name": "فیزیوتراپی",
                "Price": 3,
                "Description": "PHYSIOTRAPY",
                "TypeName": "بیمارستان امام حسین کرمانشاه",
                "Code": "000013000573"
                },
                {
                "TerminalCardID": 100009,
                "Name": "رادیولوژی",
                "Price": 4,
                "Description": "RADIOLOGY",
                "TypeName": "بیمارستان امام حسین کرمانشاه",
                "Code": "000013000574"
                },
                {
                "TerminalCardID": 100009,
                "Name": "آندوسکپی",
                "Price": 5,
                "Description": "ENDOSCOPY",
                "TypeName": "بیمارستان امام حسین کرمانشاه",
                "Code": "000013000575"
                },
                {
                "TerminalCardID": 100009,
                "Name": "بستری",
                "Price": 6,
                "Description": "ADMISSION",
                "TypeName": "بیمارستان امام حسین کرمانشاه",
                "Code": "000013000576"
                },
                {
                "TerminalCardID": 100009,
                "Name": "داروخانه",
                "Price": 7,
                "Description": "PHARMACY",
                "TypeName": "بیمارستان امام حسین کرمانشاه",
                "Code": "000013000577"
                },
                {
                "TerminalCardID": 100009,
                "Name": "آزمایشگاه",
                "Price": 8,
                "Description": "LABRATORY",
                "TypeName": "بیمارستان امام حسین کرمانشاه",
                "Code": "000013000578"
                },
                {
                "TerminalCardID": 100009,
                "Name": "پاتولوژی",
                "Price": 9,
                "Description": "PATHOLOGY",
                "TypeName": "بیمارستان امام حسین کرمانشاه",
                "Code": "000013000579"
                },
                {
                "TerminalCardID": 100009,
                "Name": "تغذیه",
                "Price": 10,
                "Description": "FEEDING",
                "TypeName": "بیمارستان امام حسین کرمانشاه",
                "Code": "000013000580"
                },
                {
                "TerminalCardID": 100009,
                "Name": "متفرقه",
                "Price": 11,
                "Description": "MISC",
                "TypeName": "بیمارستان امام حسین کرمانشاه",
                "Code": "000013000581"
                }
            ],
            "RegionID": 1571,
            "WebServiceUrl": null,
            "EnTerminalGroupTitle": "Hospital Payment",
            "FaTerminalGroupTitle": "پرداخت بیمارستان",
            "TerminalGroupDescription": "STVM=1"
            }
        }
    )json";


    Pattern terminalCardPattern
        {"Parent",false,
            {
                Pattern{"IsSuccess"},
                Pattern{"ErrorMessage"},
                Pattern{"Data",false,
                    {
                    Pattern{"Details",true,
                        {
                            Pattern{"0",true,
                                {
                                   Pattern{"Name"},
                                   Pattern{"Price"},
                                   Pattern{"Description"},
                                   Pattern{"TypeName"},
                                   Pattern{"Code"},
                                   Pattern{"TerminalCardID"}
                                }
                            }
                        }
                    }
                }
            }
        }
    };


//    TransactionInfo trxInfo{"./Parent/Data"};
//    VasMenuInfo vasInfo{"./Parent/Data/Details/TerminalCardDetail"};
    // RequestInfo info{"./Parent", {"IsSuccess"}};

    enum class VasMenuAttribute {
        MenuId,
        Name,
        Price,
        Description,
        TypeName,
        Code,
        ID,
        Counter,
        COUNT // Keep last for iteration
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

    StructInfo<VasMenuAttribute> structInfo{"./Parent/Data/Details/0",attributeNames};


    std::unique_ptr<par::IParser> parser_json = par::create_parser(&terminalCardPattern, InterpretType::JSON);
    parser_json->parse({structInfo},sampleJSON);

    std::cout << structInfo;

    return 0;
}
