#!/bin/bash
g++ GenerateIECode.cpp -std=c++11 -o GenerateIECode.out && ./GenerateIECode.out && mv *.c ../../src/amf/nas/ies/ && mv *.h ../../src/amf/nas/ies/ && sudo ../../scripts/build_amf
