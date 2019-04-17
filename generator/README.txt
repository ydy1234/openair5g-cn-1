# Generate Code of NAS messages and IEs
author: puzeyu

There are two programs to reduce the cumbersome and repetitive coding of NAS messages and IEs in 5G Core network. We will talk each splitly.

## IEs
IEs comes first on the case that NAS messages are composed of IEs. We generate IE's code using the "GenerateIECode.cpp".

compile:

    g++ GenerateIECode.cpp -o GenerateIECode.out

running:

    ./GenerateIECode.out

Input the IE's name splited into words with an extra "exit" (which means for "PaaCa", you should input "Paa","Ca","exit"; for "NASMessage", input "NAS","Message","exit"). The "exit" just ends the input of the IE's name.
Then you should input whether the IE contains IEI and length and elements.(Refer for the 3GPP TS24.501 etc.)
We get a *.c and a *.h(* is the IE name you input in the beginning.).
###ATTENTION:The code defaultly regards the length of IEI and length element as ONE BTYE! While in some IE the length element is 2 BYTEs, you should edit the code manually. There are such IEs done which you can refer to.


## NASMessage


NAS Message doesn't generate codes from input but from data in codes(I think data file is better but I haven't get down to that). To generate NAS Messages we have two files named "GenerateNASMessage.cpp" and "GenerateNASMessage.h".
There is a main function which decides which NAS files to generate. Use the function "Generate()" to generate the NAS message you wanted. The parameter is the name of the NAS message. Before use the "Generate()", declare which IEs are included in the NAS message(refer the TS24.XXX).

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


## SO
Genrate the NAS, if IE doesn's exists. Generate it.
