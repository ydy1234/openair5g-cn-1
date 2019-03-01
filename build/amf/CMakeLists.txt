cmake_minimum_required (VERSION 3.2)

set (OPENAIR5GCN_DIR  $ENV{OPENAIR5GCN_DIR})
set (BUILD_TOP_DIR    ${OPENAIR5GCN_DIR}/build)
set (OPENAIR5GCN_BIN_DIR  ${CMAKE_CURRENT_BINART_DIR}${CMAKE_FILES_DIRECTORY})
message(${OPENAIR5GCN_BIN_DIR})
project(amf)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -pg -g3 -fpermissive" )
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS} -fstack-protector-all -g -DMALLOC_CHECK_=3 -DDEBUG_IS_ON=1 -DTRACE_IS_ON=1 -O0")
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
    ${NGAP_ASN_DIR}/NGAP-PDU.asn

    )
set (NGAP_C_DIR ${OPENAIR5GCNDIR}/build/amf/build/${ASN1RELDIR})
#set (NGAP_C_DIR ${asn1_generated_dir}/${ASN1RELDIR})
#execute_process(COMMAND ${asn1c_call} ${NGAP_C_DIR} ${NGAP_ASN_FILES})
#file(GLOB NGAP_source ${NGAP_C_DIR}/*.c)
message(${NGAP_C_DIR})
set(NGAP_OAI_generated
  ${NGAP_C_DIR}/ngap_decoder.c
  ${NGAP_C_DIR}/ngap_encoder.c
  ${NGAP_C_DIR}/ngap_xer_print.c
  ${NGAP_C_DIR}/ngap_ies_defs.h
  )
#file(GLOB ngap_h ${NGAP_C_DIR}/*.h)
#add_custom_command (
#  OUTPUT ${NGAP_OAI_generated}
#  COMMAND ${asn1c_call} ${NGAP_C_DIR} ${NGAP_ASN_FILES}
#  COMMAND python ${NGAP_DIR}/messages/asn1/asn1tostruct.py -f${NGAP_ASN_DIR}/NGAP-PDU-Contents.asn -o${NGAP_C_DIR}
#  DEPENDS ${NGAP_ASN_FILES}
#  )

include_directories ("${NGAP_C_DIR}")

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

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../../src/amf/comm/api)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../../src/amf/comm/impl)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../../src/amf/evet/api)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../../src/amf/evet/impl)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../../src/amf/loc/api)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../../src/amf/loc/impl)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../../src/amf/mt/api)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../../src/amf/mt/impl)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../../src/amf/amf-models)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../../src/amf/comm/common)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../../src/amf/comm/sctp-5g)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../../src/amf/comm/utils)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../../src/amf/comm/NG-AP)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../../build/amf/build/r15.2)
#include_directories("${NGAP_C_DIR}")





#file(GLOB ASN1_C_FILES ${NGAP_C_DIR}/*.c)
file(GLOB ASN1_C_FILES ${CMAKE_CURRENT_SOURCE_DIR}/../../build/amf/build/r15.2/*.c)

file(GLOB SRCS
    ${CMAKE_CURRENT_SOURCE_DIR}/../../src/amf/comm/api/*.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/../../src/amf/comm/impl/*.cpp
    #${CMAKE_CURRENT_SOURCE_DIR}/../../src/amf/evet/api/*.cpp
    #${CMAKE_CURRENT_SOURCE_DIR}/../../src/amf/evet/impl/*.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/../../src/amf/loc/api/*.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/../../src/amf/loc/impl/*.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/../../src/amf/mt/api/*.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/../../src/amf/mt/impl/*.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/../../src/amf/amf-models/*.cpp
    ${OPENAIR5GCN_DIR}/src/amf/amf-server.cpp
    ${OPENAIR5GCN_DIR}/src/amf/comm/sctp-5g/sctp_itti_messaging.c
    ${OPENAIR5GCN_DIR}/src/amf/comm/sctp-5g/sctp_primitives_server.c
    ${OPENAIR5GCN_DIR}/src/amf/comm/utils/conversions.c
    ${OPENAIR5GCN_DIR}/src/amf/comm/sctp-5g/sctp_common.c
    ${OPENAIR5GCN_DIR}/src/amf/comm/NG-AP/ngap_amf.c
    ${OPENAIR5GCN_DIR}/src/amf/comm/NG-AP/ngap_common.c
    ${OPENAIR5GCN_DIR}/src/amf/comm/NG-AP/ngap_amf_handlers.c
    ${OPENAIR5GCN_DIR}/src/amf/comm/NG-AP/ngap_amf_decoder.c
    ${OPENAIR5GCN_DIR}/src/amf/comm/utils/log.c
    ${OPENAIR5GCN_DIR}/src/amf/comm/utils/hashtable.c
    ${OPENAIR5GCN_DIR}/src/amf/comm/utils/obj_hashtable.c
    #${CMAKE_CURRENT_SOURCE_DIR}/../../build/amf/build/r15.2/ngap_decoder.c
    #${CMAKE_CURRENT_SOURCE_DIR}/../../build/amf/build/r15.2/ngap_encoder.c
    #${CMAKE_CURRENT_SOURCE_DIR}/../../build/amf/build/r15.2/ngap_xer_print.c
    #${CMAKE_CURRENT_SOURCE_DIR}/../../build/amf/build/r15.2/*.c
    ${OPENAIR5GCN_DIR}/build/amf/build/r15.2/*.c
)

add_executable(${PROJECT_NAME} ${SRCS} ${ITTICFiles} ${ASN1_C_FILES})
target_link_libraries(${PROJECT_NAME} pistache pthread rt ${LFDS} sctp)
