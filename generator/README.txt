# Generate Code of NAS messages and IEs

Date: 2019.04.26 15:13
Author: puzeyu


There are two programs to reduce the cumbersome and repetitive coding of NAS messages and IEs in 5G Core network. We will talk each splitly.

## IEs
IEs comes first on the case that NAS messages are composed of IEs. We generate IE's code using the "GenerateIECode.cpp".

compile:

    g++ GenerateIECode.cpp -std=c++11 -o GenerateIECode.out

running:

    ./GenerateIECode.out

Every time you run the program, it will generate every IEs defined in the program in the current folder.
Define the structure of IE in the GenerateIECode.h file. There is a vector named "inputs". The "inputs" is consisted of the structure named "Input".

    {
        {//IE's name splited with words, particularly regard the abbreviation as one single word such as "5GMM","GPRS", "PDU".
            "Operator",
            "Defined",
            "Access",
            "Category",
            "Definitions"
        },
        3,//Minimum length of the IE
        9999,//Maximum length of the IE
        true,//IEI exists?
        2//length of the length part of the IE. Three options: 0, 1, 2
    }


## NASMessage


compile:

    g++ GenerateNASMessage.cpp -std=c++11 -o GenerateIECode.out

running:

    ./GenerateNASMessage.out

Every time you run the program, it will generate every NAS messages defined in the program in the current folder.
NAS Message generate codes from from data in codes. To generate NAS Messages we have two files named "GenerateNASMessage.cpp" and "GenerateNASMessage.hpp".
There is a main function which decides which NAS files to generate. Use the function "Generate()" to generate the NAS message you wanted(We haved done it, what you need to do is just declare the structure of the nas message). The parameter is the name of the NAS message. Before use the "Generate()", declare which IEs are included in the NAS message(refer the TS24.XXX).

    GenerateFile("AuthenticationRequest");

Declare it in the nas_init() in the same file. Below is an example. Easy to understand.

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

We get two files with NAS message name in the beginning.


## Special
AccessType 
PDUSessionType -> _PDUSessionType
The IE name "AccessType" duplicates with the name of the file in the AMF model source codes(See below). So we rename "AccessType.*" to "_AccessType.*".

    user@host:~/openair5g-cn$ find . -name "AccessType.*"
    ./src/amf/evet/model/AccessType.h
    ./src/amf/evet/model/AccessType.cpp
    ./src/amf/amf-models/AccessType.h
    ./src/amf/amf-models/AccessType.cpp
    ./src/amf/comm/model/AccessType.h
    ./src/amf/comm/model/AccessType.cpp

    user@host:~/openair5g-cn$~/openair5g-cn$ find -name "PDUSessionType.h"
    ./src/amf/nas/ies/PDUSessionType.h
    ./build/amf/build/r15.2/PDUSessionType.h

