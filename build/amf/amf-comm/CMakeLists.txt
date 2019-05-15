cmake_minimum_required (VERSION 3.2)

set (OPENAIR5GCN_DIR  $ENV{OPENAIR5GCN_DIR})
set (BUILD_TOP_DIR    ${OPENAIR5GCN_DIR}/build)
set (OPENAIR5GCN_BIN_DIR  ${CMAKE_CURRENT_BINART_DIR}${CMAKE_FILES_DIRECTORY})
message(${OPENAIR5GCN_BIN_DIR})
project(amf-comm)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -pg -g3 -fpermissive" )
########################################################
## ASN.1 grammer C code
#######################################################
set (asn1c_call "${BUILD_TOP_DIR}/tools/generate_asn1")
set (asn1_generated_dir ${OPENAIR5GCN_BIN_DIR})

set (NGAP_DIR ${OPENAIR5GCN_DIR}/src/amf/comm/NG-AP)
set (ASN1RELDIR r15.2)
set (NGAP_ASN_DIR ${NGAP_DIR}/messages/asn1/${ASN1RELDIR})
set (NGAP_ASN_FILES 
    ${NGAP_ASN_DIR}/NGAP-CommonDataTypes.asn
    ${NGAP_ASN_DIR}/NGAP-Constants.asn
    ${NGAP_ASN_DIR}/NGAP-IEs.asn
    ${NGAP_ASN_DIR}/NGAP-Containers.asn
    )
set (NGAP_C_DIR ${OPENAIR5GCNDIR}/build/amf/amf-comm/build/ASN1RELDIR)
############################################################

###########################################################
# ITTI 
#########################################################
set(ittiPath ${OPENAIR5GCN_DIR}/src/amf/comm/ittiFiles)
set(ITTICFiles
    ${ittiPath}/inc/backtrace.c
    ${ittiPath}/inc/bstrlib.c
    ${ittiPath}/inc/intertask_interface.c
    ${ittiPath}/inc/intertask_interface_dump.c
    ${ittiPath}/inc/memory_pools.c
    ${ittiPath}/inc/signals.c
    ${ittiPath}/inc/timer.c
    ${ittiPath}/inc/dynamic_memory_check.c
)
set(LFDS ${ittiPath}/lib/liblfds611.a)
include_directories(${ittiPath}/inc)

########################################################
include_directories(/usr/local/include/nlohmann)
include_directories(/usr/local/include/pistache)
link_directories(/usr/local/lib)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../../../src/amf/comm/model)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../../../src/amf/comm/api)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../../../src/amf/comm/impl)

file(GLOB SRCS
    ${CMAKE_CURRENT_SOURCE_DIR}/../../../src/amf/comm/api/*.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/../../../src/amf/comm/impl/*.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/../../../src/amf/comm/model/*.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/../../../src/amf/comm/*.cpp
)

add_executable(${PROJECT_NAME} ${SRCS} ${ITTICFiles})
target_link_libraries(${PROJECT_NAME} pistache pthread rt ${LFDS})
