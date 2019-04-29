#!/bin/bash 
exec="GenerateNASMessageCode.out"
sour="GenerateNASMessageCode.cpp"
g++ $sour -std=c++11 -o $exec
echo "Compile "$exec
./$exec && mv *.c ../../src/amf/nas/mm/msg/ && mv *.h ../../src/amf/nas/mm/msg/ && sudo ../../scripts/build_amf
