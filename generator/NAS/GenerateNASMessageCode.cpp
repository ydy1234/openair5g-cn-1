#include <iostream>
#include <cstring>
#include <fstream>
#include <unordered_map>
#include <string>
#include <vector>


using namespace std;

unordered_map<string,vector<string>> ies;
unordered_map<string,vector<string>> nas;

int ies_init();
int nas_init();

void Dump();

void GenerateFile(const string str);

vector<string> BreakString(string str);

bool IsLower(char ch);
bool IsUpper(char ch);
bool IsDigit(char ch);

string FileName(const string str);
string HFileName(const string str);
string CFileName(const string str);
string DataName(const string str);
string DataTypeName(const string str);

string DefineName(const string str);
string DefineNameMini(const string str);
string DefineNameMaxi(const string str);

string FunctionName(const string str);
string FunctionNameEncode(const string str);
string FunctionNameDecode(const string str);


string Upper(const string str);
string Lower(const string str);

char Upper(const char ch);
char Lower(const char ch);
int ies_init()
{
    return 0;
}



int nas_init()
{

    //MM head
    //{
        //nas["AuthenticationRequest"]=
        //{
            //"ExtendedProtocolDiscriminator",
            //"SecurityHeaderType",
            //"MessageType",
            //"NASKeySetIdentifier",
            //"ABBA",
            //"AuthenticationParameterRAND",
            //"AuthenticationParameterAUTN",
            //"EAPMessage"
        //};


        //nas["AuthenticationResponse"]=
        //{
            //"ExtendedProtocolDiscriminator",
            //"SecurityHeaderType",
            //"MessageType",
            //"NASKeySetIdentifier",
            //"ABBA",
            //"AuthenticationParameterRAND",
            //"AuthenticationParameterAUTN",
            //"AuthenticationResponseParameter",
            //"EAPMessage"
        //};

        //nas["AuthenticationResult"]=
        //{
            //"ExtendedProtocolDiscriminator",
            //"SecurityHeaderType",
            //"MessageType",
            //"NASKeySetIdentifier",
            //"EAPMessage",
            //"ABBA"
        //};

        //nas["AuthenticationFailure"]=
        //{
            //"ExtendedProtocolDiscriminator",
            //"SecurityHeaderType",
            //"MessageType",
            //"_5GMMCause",
            //"AuthenticationFailureParameter"
        //};

        //nas["AuthenticationReject"]=
        //{
            //"ExtendedProtocolDiscriminator",
            //"SecurityHeaderType",
            //"MessageType",
            //"EAPMessage"
        //};

        //nas["RegistrationRequest"]=
        //{
            //"ExtendedProtocolDiscriminator",
            //"SecurityHeaderType",
            //"MessageType",
            //"_5GSRegistrationType",
            //"NASKeySetIdentifier",
            //"_5GSMobileIdentity",
            ////"NASKeySetIdentifier",
            //"_5GMMCapability",
            //"UESecurityCapability",
            //"NSSAI",
            //"_5GSTrackingAreaIdentity",
            //"S1UENetworkCapability",
            //"UplinkDataStatus",
            //"PDUSessionStatus",
            //"MICOIndication",
            //"UEStatus",
            ////"_5GSMobileIdentity",
            //"AllowedPDUSessionStatus",
            //"UESUsageSetting",
            //"_5GSDRXParameters",
            //"EPSNASMessageContainer",
            //"LADNIndication",
            //"PayloadContainer",
            //"NetworkSlicingIndication",
            //"_5GSUpdateType",
            //"NASMessageContainer"
        //};

        //nas["RegistrationAccept"]=
        //{
            //"ExtendedProtocolDiscriminator",
            //"SecurityHeaderType",
            //"MessageType",
            //"_5GSRegistrationResult",
            //"_5GSMobileIdentity",
            //"PLMNList",
            //"_5GSTrackingAreaIdentityList",
            //"NSSAI",
            //"RejectedNSSAI",
            //"_5GSNetworkFeatureSupport",
            //"PDUSessionStatus",
            //"PDUSessionReactivationResult",
            //"PDUSessionReactivationResultErrorCause",
            //"LADNInformation",
            //"MICOIndication",
            //"NetworkSlicingIndication",
            //"ServiceAreaList",
            //"GPRSTimer3",
            //"GPRSTimer2",
            //"EmergencyNumberList",
            //"ExtendedEmergencyNumberList",
            //"SORTransparentContainer",
            //"EAPMessage",
            //"NSSAIInclusionMode",
            //"OperatorDefinedAccessCategoryDefinitions",
            //"_5GSDRXParameters"
        //};

        //nas["RegistrationReject"]=
        //{
            //"ExtendedProtocolDiscriminator",
            //"SecurityHeaderType",
            //"MessageType",
            //"_5GMMCause",
            //"GPRSTimer2",
            //"EAPMessage"
        //};

        //nas["ULNASTransport"]=
        //{
            //"ExtendedProtocolDiscriminator",
            //"SecurityHeaderType",
            //"MessageType",
            //"PayloadContainerType",
            //"PayloadContainer",
            //"PDUSessionIdentity2",
            ////"PDUSessionIdentity2",
            //"RequestType",
            //"SNSSAI",
            //"DNN",
            //"AdditionalInformation"
        //};

        //nas["DLNASTransport"]=
        //{
            //"ExtendedProtocolDiscriminator",
            //"SecurityHeaderType",
            //"MessageType",
            //"PayloadContainerType",
            //"PayloadContainer",
            //"PDUSessionIdentity2",
            //"AdditionalInformation",
            //"_5GMMCause",
            //"GPRSTimer3"
        //};

        //nas["DeregistrationRequest"]=
        //{
            //"ExtendedProtocolDiscriminator",
            //"SecurityHeaderType",
            //"MessageType",
            //"DeregistrationType",
            //"NASKeySetIdentifier",
            //"_5GSMobileIdentity"
        //};

        //nas["DeregistrationAccept"]=
        //{
            //"ExtendedProtocolDiscriminator",
            //"SecurityHeaderType",
            //"MessageType"
        //};

        //nas["ServiceRequest"]=
        //{
            //"ExtendedProtocolDiscriminator",
            //"SecurityHeaderType",
            //"MessageType",
            //"NASKeySetIdentifier",
            //"ServiceType",
            //"_5GSMobileIdentity",
            //"UplinkDataStatus",
            //"PDUSessionStatus",
            //"AllowedPDUSessionStatus",
            //"NASMessageContainer"
        //};

        //nas["ServiceAccept"]=
        //{
            //"ExtendedProtocolDiscriminator",
            //"SecurityHeaderType",
            //"MessageType",
            //"PDUSessionStatus",
            //"PDUSessionReactivationResult",
            //"PDUSessionReactivationResultErrorCause",
            //"EAPMessage"
        //};

        //nas["ServiceReject"]=
        //{
            //"ExtendedProtocolDiscriminator",
            //"SecurityHeaderType",
            //"MessageType",
            //"_5GMMCause",
            //"PDUSessionStatus",
            //"GPRSTimer2",
            //"EAPMessage"
        //};

        //nas["ConfigurationUpdateCommand"]=
        //{
            //"ExtendedProtocolDiscriminator",
            //"SecurityHeaderType",
            //"MessageType",
            //"ConfigurationUpdateIndication",
            //"_5GSMobileIdentity",
            //"_5GSTrackingAreaIdentityList",
            //"NSSAI",
            //"ServiceAreaList",
            //"NetworkName",
            //"TimeZone",
            //"TimeZoneAndTime",
            //"DaylightSavingTime",
            //"LADNInformation",
            //"MICOIndication",
            //"NetworkSlicingIndication",
            //"RejectedNSSAI",
            //"OperatorDefinedAccessCategoryDefinitions",
            //"SMSIndication"
        //};

        //nas["ConfigurationUpdateComplete"]=
        //{
            //"ExtendedProtocolDiscriminator",
            //"SecurityHeaderType",
            //"MessageType"
        //};

        //nas["IdentityRequest"]=
        //{
            //"ExtendedProtocolDiscriminator",
            //"SecurityHeaderType",
            //"MessageType",
            //"_5GSIdentityType"
        //};

        //nas["IdentityResponse"]=
        //{
            //"ExtendedProtocolDiscriminator",
            //"SecurityHeaderType",
            //"MessageType",
            //"_5GSMobileIdentity"
        //};

        //nas["Notification"]=
        //{
            //"ExtendedProtocolDiscriminator",
            //"SecurityHeaderType",
            //"MessageType",
            //"_AccessType"
        //};

        //nas["NotificationResponse"]=
        //{
            //"ExtendedProtocolDiscriminator",
            //"SecurityHeaderType",
            //"MessageType",
            //"PDUSessionStatus"
        //};

        //nas["SecurityModeCommand"]=
        //{
            //"ExtendedProtocolDiscriminator",
            //"SecurityHeaderType",
            //"MessageType",
            //"NASSecurityAlgorithms",
            //"NASKeySetIdentifier",
            //"UESecurityCapability",
            //"IMEISVRequest",
            //"EPSNASSecurityAlgorithms",
            //"Additional5GSecurityInformation",
            //"EAPMessage",
            //"ABBA",
            //"S1UESecurityCapability"
        //};

        //nas["SecurityModeComplete"]=
        //{
            //"ExtendedProtocolDiscriminator",
            //"SecurityHeaderType",
            //"MessageType",
            //"_5GSMobileIdentity",
            //"NASMessageContainer"
        //};

        //nas["SecurityModeReject"]=
        //{
            //"ExtendedProtocolDiscriminator",
            //"SecurityHeaderType",
            //"MessageType",
            //"_5GMMCause"
        //};

        //nas["SecurityProtected5GSNASMessage"]=
        //{
            //"ExtendedProtocolDiscriminator",
            //"SecurityHeaderType",
            //"MessageAuthenticationCode",
            //"SequenceNumber",
            //"Plain5GSNASMessage"
        //};

        //nas["_5GMMStatus"]=
        //{
            //"ExtendedProtocolDiscriminator",
            //"SecurityHeaderType",
            //"MessageType",
            //"_5GMMCause"
        //};

        //nas["RegistrationComplete"]=
        //{
            //"ExtendedProtocolDiscriminator",
            //"SecurityHeaderType",
            //"MessageType",
            //"SORTransparentContainer"
        //};
    //}
    //MM end


    //SM head
    {
        nas["PDUSessionEstablishmentRequest"]=
        {
            "ExtendedProtocolDiscriminator",
            "PDUSessionIdentity",
            "ProcedureTransactionIdentity",
            "MessageType",
            "IntergrityProtectionMaximumDataRate",
            "_PDUSessionType",
            "SSCMode",
            "_5GSMCapability",
            "MaximumNumberOfSupportedPacketFilters",
            "AlwaysonPDUSessionRequested",
            "SMPDUDNRequestContainer",
            "ExtendedProtocolConfigurationOptions"
        };

        nas["PDUSessionEstablishmentAccept"]=
        {
            "ExtendedProtocolDiscriminator",
            "PDUSessionIdentity",
            "ProcedureTransactionIdentity",
            "MessageType",
            "_PDUSessionType",
            "SSCMode",
            "QOSRules",
            "SessionAMBR",
            "_5GSMCause",
            "PDUAddress",
            "GPRSTimer",
            "SNSSAI",
            "AlwaysonPDUSessionIndication",
            "MappedEPSBearerContexts",
            "EAPMessage",
            "QOSFlowDescriptions",
            "ExtendedProtocolConfigurationOptions",
            "DNN"
        };

        nas["PDUSessionEstablishmentReject"]=
        {
            "ExtendedProtocolDiscriminator",
            "PDUSessionIdentity",
            "ProcedureTransactionIdentity",
            "MessageType",
            "_5GSMCause",
            "GPRSTimer3",
            "AllowedSSCMode",
            "EAPMessage",
            "ExtendedProtocolConfigurationOptions"
        };

        nas["PDUSessionAuthenticationCommand"]=
        {
            "ExtendedProtocolDiscriminator",
            "PDUSessionIdentity",
            "ProcedureTransactionIdentity",
            "MessageType",
            "EAPMessage",
            "ExtendedProtocolConfigurationOptions"
        };

        nas["PDUSessionAuthenticationComplete"]=
        {
            "ExtendedProtocolDiscriminator",
            "PDUSessionIdentity",
            "ProcedureTransactionIdentity",
            "MessageType",
            "EAPMessage",
            "ExtendedProtocolConfigurationOptions"
        };

        nas["PDUSessionAuthenticationResult"]=
        {
            "ExtendedProtocolDiscriminator",
            "PDUSessionIdentity",
            "ProcedureTransactionIdentity",
            "MessageType",
            "EAPMessage",
            "ExtendedProtocolConfigurationOptions"
        };

        nas["PDUSessionModificationRequest"]=
        {
            "ExtendedProtocolDiscriminator",
            "PDUSessionIdentity",
            "ProcedureTransactionIdentity",
            "MessageType",
            "_5GSMCapability",
            "_5GSMCause",
            "MaximumNumberOfSupportedPacketFilters",
            "AlwaysonPDUSessionRequested",
            "IntergrityProtectionMaximumDataRate",
            "QOSRules",
            "QOSFlowDescriptions",
            "MappedEPSBearerContexts",
            "ExtendedProtocolConfigurationOptions"
        };

        nas["PDUSessionModificationReject"]=
        {
            "ExtendedProtocolDiscriminator",
            "PDUSessionIdentity",
            "ProcedureTransactionIdentity",
            "MessageType",
            "_5GSMCause",
            "GPRSTimer3",
            "ExtendedProtocolConfigurationOptions"
        };

        nas["PDUSessionModificationCommand"]=
        {
            "ExtendedProtocolDiscriminator",
            "PDUSessionIdentity",
            "ProcedureTransactionIdentity",
            "MessageType",
            "_5GSMCause",
            "SessionAMBR",
            "GPRSTimer",
            "AlwaysonPDUSessionIndication",
            "QOSRules",
            "MappedEPSBearerContexts",
            "QOSFlowDescriptions",
            "ExtendedProtocolConfigurationOptions"
        };

        nas["PDUSessionModificationComplete"]=
        {
            "ExtendedProtocolDiscriminator",
            "PDUSessionIdentity",
            "ProcedureTransactionIdentity",
            "MessageType",
            "ExtendedProtocolConfigurationOptions"
        };

        nas["PDUSessionModificationCommandReject"]=
        {
            "ExtendedProtocolDiscriminator",
            "PDUSessionIdentity",
            "ProcedureTransactionIdentity",
            "MessageType",
            "_5GSMCause",
            "ExtendedProtocolConfigurationOptions"
        };

        nas["PDUSessionReleaseRequest"]=
        {
            "ExtendedProtocolDiscriminator",
            "PDUSessionIdentity",
            "ProcedureTransactionIdentity",
            "MessageType",
            "_5GSMCause",
            "ExtendedProtocolConfigurationOptions"
        };

        nas["PDUSessionReleaseReject"]=
        {
            "ExtendedProtocolDiscriminator",
            "PDUSessionIdentity",
            "ProcedureTransactionIdentity",
            "MessageType",
            "_5GSMCause",
            "ExtendedProtocolConfigurationOptions"
        };

        nas["PDUSessionReleaseCommand"]=
        {
            "ExtendedProtocolDiscriminator",
            "PDUSessionIdentity",
            "ProcedureTransactionIdentity",
            "MessageType",
            "_5GSMCause",
            "GPRSTimer3",
            "EAPMessage",
            "ExtendedProtocolConfigurationOptions"
        };

        nas["PDUSessionReleaseComplete"]=
        {
            "ExtendedProtocolDiscriminator",
            "PDUSessionIdentity",
            "ProcedureTransactionIdentity",
            "MessageType",
            "_5GSMCause",
            "ExtendedProtocolConfigurationOptions"
        };

        nas["_5GSMStatus"]=
        {
            "ExtendedProtocolDiscriminator",
            "PDUSessionIdentity",
            "ProcedureTransactionIdentity",
            "MessageType",
            "_5GSMCause"
        };
    }
    //sm end


    for(auto it=nas.begin();it!=nas.end();it++)
    {
        for(auto str:it->second)
        {
            if(ies.find(str)==ies.end())
            {
                ies[str]=BreakString(str);
            }
        }
    }
}

void Dump()
{
    for(auto it=nas.begin();it!=nas.end();it++)
    {
        cout<<it->first<<":\n";
        for(auto str:it->second)
        {
            cout<<'\t'<<str<<'\n';
            cout<<"\t\t";
            for(auto ie:ies[str])
            {
                cout<<ie<<' ';
            }
            cout<<endl;
        }
        cout<<endl;
    }
}

void GenerateFile(const string message)//
{

    //.h

    ofstream fh;
    fh.open(HFileName(message));
    fh<<"\
#include <stdint.h>\n\n";
    for(auto str:nas[message])
    {
        fh<<"\
#include \""+HFileName(str)<<"\"\n";
    }

    fh<<"\n\n";

    fh<<"/* Minimum length macro. Formed by minimum length of each mandatory field */\n";

    fh<<"\
#define "+DefineNameMini(message)+ " ( \\\n\
";


    for(auto str:nas[message])
    {
        fh<<"\
\t\t"+DefineNameMini(str) +" + \\\n\
";
    }

    fh<<"0)\n\n";


    //Maxi
    //
    fh<< "/* Maximum length macro. Formed by maximum length of each field */\n";
    fh<<"\
#define "+DefineNameMaxi(message)+ " ( \\\n\
";


    for(auto str:nas[message])
    {
        fh<<"\
\t\t"+DefineNameMaxi(str) +" + \\\n\
";
    }

    fh<<"0)\n\n";

    //struct
    fh<<"\
typedef struct "<<DataTypeName(message)+"_tag{\n";

    for(auto str:nas[message])
    {
        fh<<"\
\t"+str+" "+Lower(str)+";\n";
    }

    fh<<"\
}"+DataTypeName(message)+";\n";

fh<<"\n\n";

//decode
fh<<"int "+FunctionNameDecode(message)+"("+DataTypeName(message)+" *"+Lower(message)+", uint8_t *buffer, uint32_t len);\n";
fh<<"int "+FunctionNameEncode(message)+"("+DataTypeName(message)+" *"+Lower(message)+", uint8_t *buffer, uint32_t len);\n";
/*int encode_authentication_request(authentication_request_msg *authenticationrequest, uint8_t *buffer, uint32_t len);*/

    fh.close();

    //.c
    ofstream fc;
    fc.open(CFileName(message));

    fc<<"\
#include <stdio.h>\n\
#include <stdlib.h>\n\
#include <string.h>\n\
#include <stdint.h>\n\
#include \"TLVEncoder.h\"\n\
#include \"TLVDecoder.h\"\n\
#include \""+HFileName(message)+"\"\n\
";

    fc<<"\n";

//decode
{
    fc<<"\
int "+FunctionNameDecode(message)+"( "+DataTypeName(message)+" *"+DataName(message)+", uint8_t* buffer, uint32_t len)\n\
{\n\
    uint32_t decoded = 0;\n\
    int decoded_result = 0;\n\
\n\
    // Check if we got a NULL pointer and if buffer length is >= minimum length expected for the message.\n\
    CHECK_PDU_POINTER_AND_LENGTH_DECODER (buffer, "+DefineNameMini(message)+", len);\n\n\
";

    for(auto str:nas[message] )
    {
        fc<<"\
    if((decoded_result = "+FunctionNameDecode(str)+" (&"+DataName(message)+"->"+Lower(str)+", 0, buffer+decoded,len-decoded))<0)\n\
        return decoded_result;\n\
    else\n\
        decoded+=decoded_result;\n\n\
";
    }

    fc<<"\n\
    return decoded;\n\
}\n\
";
}

fc<<"\n\n";
//encode
{
    fc<<"\
int "+FunctionNameEncode(message)+"( "+DataTypeName(message)+" *"+DataName(message)+", uint8_t* buffer, uint32_t len)\n\
{\n\
    uint32_t encoded = 0;\n\
    int encoded_result = 0;\n\
    \n\
    // Check if we got a NULL pointer and if buffer length is >= minimum length expected for the message.\n\
    CHECK_PDU_POINTER_AND_LENGTH_ENCODER (buffer, "+DefineNameMini(message)+", len);\n\n\
";

            for(auto str:nas[message] )
            {
                fc<<"\
    if((encoded_result = "+FunctionNameEncode(str)+" ("+DataName(message)+"->"+Lower(str)+", 0, buffer+encoded,len-encoded))<0)\n\
        return encoded_result;\n\
    else\n\
        encoded+=encoded_result;\n\n\
";
            }

            fc<<"\n\
    return encoded;\n\
}\n\
";
}

    fc.close();
    cout<<"Generate Files:message"<<message<<"\n"<<CFileName(message)<<'\t'<<HFileName(message)<<endl;

}


bool IsLower(char ch)
{
    return ch>='a'&&ch<='z';
}

bool IsUpper(char ch)
{
    return ch>='A'&&ch<='Z';
}

bool IsDigit(char ch)
{
    return ch>='0'&&ch<='9';
}

string FileName(const string str)
{
    return str;
}

string HFileName(const string str)
{
    string rtn=FileName(str);
    if(rtn=="")
    {
        return rtn;
    }
    return rtn+".h";
}
string CFileName(const string str)
{
    string rtn=FileName(str);
    if(rtn=="")
    {
        return rtn;
    }
    return rtn+".c";
}

string DataName(const string str)
{
    vector<string> strs=BreakString(str);
    string rtn;
    for(auto str:strs)
    {
        rtn=rtn+Lower(str)+"_";
    }
    if(rtn!="") rtn.pop_back();
    return rtn;
}

string DataTypeName(const string str)
{
    /*vector<string> strs;*/
    /*if(ies.find(str)==ies.end())*/
    /*{*/
        /*if(nas.find(str)==nas.end())*/
        /*{*/
            /*return "";*/
        /*}else//nas*/
        /*{*/
            /*strs=nas[str];*/
        /*}*/
    /*}else//ies*/
    /*{*/
        /*strs=ies[str];*/
    /*}*/
    /*string rtn="";*/
    /*for(auto str:strs)*/
    /*{*/
        /*rtn=rtn+str;*/
    /*}*/
    vector<string> strs=BreakString(str);
    if(strs.size()!=0) strs.push_back("msg");
    string rtn;
    for(auto str:strs)
    {
        rtn=rtn+Lower(str)+"_";
    }
    if(rtn!="") rtn.pop_back();
    return rtn;
}

string DefineName(const string str)
{
    string rtn="";
    auto strs=BreakString(str);
    for(auto str:strs)
    {
        rtn=rtn+Upper(str)+'_';
    }

    return rtn;

}

string DefineNameMini(const string str)
{
    string rtn=DefineName(str);
    return rtn==""?rtn:(rtn+"MINIMUM_LENGTH");
}

string DefineNameMaxi(const string str)
{
    string rtn=DefineName(str);
    return rtn==""?rtn:(rtn+"MAXIMUM_LENGTH");
}

string FunctionName(const string str)
{
    string rtn;
    vector<string> strs=BreakString(str);
    /*if(ies.find(str)!=ies.end())*/
    /*{*/
        /*strs=ies[str];*/
    /*}else if(nas.find(str)!=nas.end())*/
    /*{*/
        /*strs=nas[str];*/
    /*}else*/
    /*{*/
        /*return "";*/
    /*}*/
    for(auto str:strs)
    {
        rtn+="_"+Lower(str);
    }
    return rtn;
}
string FunctionNameEncode(const string str)
{
    string rtn=FunctionName(str);
    return rtn==""?rtn:("encode"+rtn);
}

string FunctionNameDecode(const string str)
{
    string rtn=FunctionName(str);
    return rtn==""?rtn:("decode"+rtn);
}



string Upper(const string str)
{
    string rtn="";
    for(auto s:str)
    {
        rtn+=Upper(s);
    }
    return rtn;
}

string Lower(const string str)
{
    string rtn="";
    for(auto s:str)
    {
        rtn+=Lower(s);
    }
    return rtn;
}



char Upper(const char ch)
{
    if(ch>='a'&&ch<='z')
        return ch-'a'+'A';
    return ch;
}

char Lower(const char ch)
{
    if(ch>='A'&&ch<='Z')
        return ch-'A'+'a';
    return ch;
}


ostream& operator<< (ostream& os, vector<string> strs)
{
    for(auto str:strs)
    {
        cout<<str<<endl;
    }
    return os;
}

vector<string> BreakString(string str)
{

    vector<string> strs;
    if(str=="_AccessType")
    {
        strs.push_back("_Access");
        strs.push_back("Type");
        return strs;
    }
    if(str.size()==0) return strs;
    int i=0;
    int j=0;
    if(str.size()!=0&&str[0]=='_')
    {
        strs.push_back("_");
        i++;
    }
    while(i<str.size())
    {

        j=i+1;
        while(j<str.size()&&!IsUpper(str[j])&&str[j]!='_')
        {
            if(j>i&&IsDigit(str[j])&&(
                        str[j-1]=='l'
                        || str[j-1]=='n'
                        ))
            {
                //j--;
                break;
            }

            j++;
        }
        strs.push_back(str.substr(i,j-i));
        i=j;
    }
    vector<string> rtn;
    string tmp;
    for(auto str:strs)
    {
        if(str.size()==1)
        {
            tmp+=str;
        }else
        {
            if(tmp.size()!=0)
            {
                rtn.push_back(tmp);
                tmp="";
            }
            rtn.push_back(str);
        }
    }
    if(tmp.size()!=0)
    {
        rtn.push_back(tmp);
        tmp="";
    }
    return rtn;

}
int main()
{
    //cout<<BreakString("Additional5GSecurityInformation");
    //cout<<BreakString("_5GSMMCause");
    //cout<<BreakString("5GSMMCause");
    //cout<<BreakString("GPRSTimer3");
    //cout<<BreakString("GPRSTimer3Like");
    //cout<<BreakString("UESUsageSetting");
    //cout<<BreakString("_AccessType");


    nas_init();

    for(auto it=nas.begin();it!=nas.end();it++)
    {
        GenerateFile(it->first);
    }

    return 0;
}
