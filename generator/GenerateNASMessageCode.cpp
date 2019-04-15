#include <iostream>
#include <cstring>
#include <fstream>
#include <unordered_map>
#include <string>
#include "GenerateNASMessageCode.h"


using namespace std;

int ies_init()
{

    return 0;
}



int nas_init()
{
    nas["AuthenticationRequest"]=
    {
        "ExtendedProtocolDiscriminator",
        "SecurityHeaderType",
        "MessageType",
        "NASKeySetIdentifier",
        "ABBA",
        "AuthenticationParameterRand",
        "AuthenticationParameterAutn",
        "EAPMessage"
    };


    nas["AuthenticationResponse"]=
    {
        "ExtendedProtocolDiscriminator",
        "SecurityHeaderType",
        "MessageType",
        /*"NASKeySetIdentifier",*/
        /*"ABBA",*/
        /*"AuthenticationParameterRAND",*/
        /*"AuthenticationParameterAUTN",*/
        "AuthenticationResponseParameter"
        /*"EAPMessage"*/
    };

    nas["AuthenticationResult"]=
    {
        "ExtendedProtocolDiscriminator",
        "SecurityHeaderType",
        "MessageType",
        "NASKeySetIdentifier",
        "EAPMessage",
        "ABBA"
    };

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
    cout<<"Generate Files:\n"<<CFileName(message)<<'\t'<<HFileName(message)<<endl;

}

vector<string> BreakString(string str)
{
    vector<string> strs;
    if(str.size()==0) return strs;
    int i=0;
    if(str[i]>='0'&&str[i]<='9')
    {
        str="_"+str;
        strs.push_back("_");
        i=1;
    }
    int j=0;
    while(i<str.size())
    {

        j=i+1;
        while(j<str.size()&&!IsUpper(str[j]))
        {
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

bool IsLower(char ch)
{
    return ch>='a'&&ch<='z';
}

bool IsUpper(char ch)
{
    return ch>='A'&&ch<='Z';
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
    /*vector<string> strs;*/
    /*if(ies.find(str)!=ies.end())*/
    /*{*/
        /*strs=ies[str];*/
    /*}else if(nas.find(str)!=ies.end())*/
    /*{*/
        /*strs=nas[str];*/
    /*}else*/
    /*{*/
        /*return "";*/
    /*}*/
    /*for(auto str:strs)*/
    /*{*/
        /*rtn+=Upper(str)+"_";*/
    /*}*/
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

int main()
{
    nas_init();

    /*Dump();*/
    GenerateFile("AuthenticationRequest");
    GenerateFile("AuthenticationResponse");
    GenerateFile("AuthenticationResult");


    return 0;
}
