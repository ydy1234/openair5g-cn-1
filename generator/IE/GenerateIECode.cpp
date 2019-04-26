#include<iostream>
#include<cstring>
#include<fstream>
#include<algorithm>
#include<queue>
#include<climits>
#include<stdio.h>
#include<unordered_map>
#include<unordered_set>
#include<unistd.h>//fork
#include<vector>//fork

#include "GenerateIECode.hpp"

using namespace std;

//auto generate codes
char Upper(const char & ch)
{
    if(ch>='a'&&ch<='z')
    {
        return ch-'a'+'A';
    }
    return ch;
}

char Lower(const char& ch)
{
    if(ch>='A'&&ch<='Z')
    {
        return ch-'A'+'a';
    }
    return ch;
}

string Upper(string str)
{
    string res;
    for(auto s:str)
    {
        res+=Upper(s);
    }
    return res;
}

string Lower(string str)
{
    string res;
    for(auto s:str)
    {
        res+=Lower(s);
    }
    return res;
}

//struct Input{
    //vector<string> strs;
    //unsigned int MIN;
    //unsigned int MAX;
    //bool ieiflag;
   //int ielenflag;
//};


int main()
{
    for(auto input:inputs)
    {
        vector<string> names;
        string filec;//name of file.c
        string fileh;//name of file.h

        names=input.strs;
        string str;
        int MIN,MAX;
        MIN=input.MIN;
        MAX=input.MAX;

        bool ieiflag=false;
        ieiflag=input.ieiflag;
        int ielenflag=0;
        ielenflag=input.ielenflag;

        if(names.size()>0&&names[0].size()>0&&names[0][0]>='0'&&names[0][0]<='9')
        {
            names[0]='_'+names[0];
        }
        str="";
        for(auto name:names)
        {
            str=str+name;
        }

        string UPPER_="";
        string lowwer_="";
        for(auto name:names)
        {
            UPPER_+=Upper(name)+'_';
            lowwer_+=Lower(name)+'_';
        }
        lowwer_.pop_back();
        filec=str+".c";
        fileh=str+".h";
        cout<<"Generate file :\n\t";
        cout<<filec<<'\t';
        cout<<fileh<<'\n'<<endl;

        string encode="int encode_"+lowwer_+" ( "+str+" "+Lower(str)+", uint8_t iei, uint8_t * buffer, uint32_t len "+" ) ";
        string decode="int decode_"+lowwer_+" ( "+str+" * "+Lower(str)+", uint8_t iei, uint8_t * buffer, uint32_t len "+" ) ";

        ofstream foutc;
        ofstream fouth;

        foutc.open(filec);
        fouth.open(fileh);
        //fouth

        vector<string> hs
        {
            "#include <stdint.h>",
                "#include \"bstrlib.h\"",
                "",
                "#define "+UPPER_+"MINIMUM_LENGTH "+to_string(MIN),
                "#define "+UPPER_+"MAXIMUM_LENGTH "+to_string(MAX),
                "",
                "typedef bstring "+str+";",
                "",
                encode+";",
                decode+";",
                ""
        };

        string ieide="";
        string ieien="";
        if(ieiflag)
        {
            ieide="\
    if (iei > 0)\n\
    {\n\
        CHECK_IEI_DECODER (iei, *buffer);\n\
        decoded++;\n\
    }\n\
";
ieien="\
       if( iei >0  )\n\
       {\n\
           *buffer=iei;\n\
               encoded++;\n\
       }\n\
";

        }

        string ielende="";
        string ielenen1="";
        string ielenen2="";
        if(ielenflag==1)
        {
ielende="\
    ielen = *(buffer + decoded);\n\
    decoded++;\n\
    CHECK_LENGTH_DECODER (len - decoded, ielen);\n\
";
ielenen1="\n\
    lenPtr = (buffer + encoded);\n\
    encoded++;\n\
";
ielenen2="\
    *lenPtr = encoded - 1 - ((iei > 0) ? 1 : 0);\
    ";
        }else if(ielenflag==2)
        {

ielende="\
    ielen = *(buffer + decoded);\n\
    decoded++;\n\
    ielen = ( ielen << 8)+*(buffer + decoded);\n\
    decoded++;\n\
    CHECK_LENGTH_DECODER (len - decoded, ielen);\n\
";
ielenen1="\n\
    lenPtr = (buffer + encoded);\n\
    encoded++;\n\
    encoded++;\n\
";
ielenen2="\
    uint32_t res = encoded - 1 - ((iei > 0) ? 1 : 0);\n\
    *lenPtr =res/(1<<8);\n\
    lenPtr++;\n\
    *lenPtr = res%(1<<8);\n\
";
        }



        vector<string> cs
        {
            "#include<stdio.h>",
                "#include<stdlib.h>",
                "#include<stdint.h>",
                "",
                "#include \"TLVEncoder.h\"",
                "#include \"TLVDecoder.h\"",
                "#include \""+fileh+"\"",
                "",
                encode,
"{\n\
    uint8_t *lenPtr;\n\
    uint32_t encoded = 0;\n\
    int encode_result;\n\
    CHECK_PDU_POINTER_AND_LENGTH_ENCODER (buffer,"+UPPER_+"MINIMUM_LENGTH , len);\n\
    ",
                "",
                ieien,
                "",
                ielenen1,
                "",
                "\n\
    if ((encode_result = encode_bstring ("+Lower(str)+", buffer + encoded, len - encoded)) < 0)//加密,实体,首地址,长度\n\
        return encode_result;\n\
    else\n\
        encoded += encode_result;\n\
",
                ielenen2,
"\
    return encoded;\n"
    "}",
"",
                decode,
                "{",
                "\tint decoded=0;",
                "\tuint8_t ielen=0;",
                "\tint decode_result;",
                "",
                ieide,
                "",
                ielende,
                "",
"\
    if((decode_result = decode_bstring ("+Lower(str)+", ielen, buffer + decoded, len - decoded)) < 0)\n\
        return decode_result;\n\
    else\n\
        decoded += decode_result;\n\
            return decoded;\n\
}",
            ""
        };

        for(auto h:hs)
        {
            fouth<<h<<'\n';
        }

        for(auto c:cs)
        {
            foutc<<c<<'\n';
        }



        foutc.close();
        fouth.close();

    }
    return 0;
}
