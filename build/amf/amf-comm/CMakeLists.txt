cmake_minimum_required (VERSION 3.2)

set (OPENAIR5GCN_DIR  $ENV{OPENAIR5GCN_DIR})
set (BUILD_TOP_DIR    ${OPENAIR5GCN_DIR}/build)
set (OPENAIR5GCN_BIN_DIR  ${CMAKE_CURRENT_BINART_DIR}${CMAKE_FILES_DIRECTORY})
message(${OPENAIR5GCN_BIN_DIR})
project(amf-comm)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -pg -g3" )

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
    #${NGAP_ASN_DIR}/NGAP-PDU-Contents.asn
    #${NGAP_ASN_DIR}/NGAP-PDU-Descriptions.asn
    )
set (NGAP_C_DIR ${OPENAIR5GCNDIR}/build/amf/amf-comm/build/GeneretedCCodeFromAsnFiles)
#set(NGAP_C_DIR "/home/core5g-dev/openair5g-cn/scripts/test")
#message("begin to generete C code from asn files")
#message(${NGAP_C_DIR})
#message(${asn1c_call})
#message(${asn1_generated_dir})
#message(${NGAP_DIR})
#message(${NGAP_ASN_DIR})
#message(${NGAP_ASN_FILES})
#execute_process (COMMAND mkdir ${asn1_generated_dir}/ASN1RELDIR)
#execute_process (COMMAND ${asn1c_call} ${NGAP_C_DIR} ${NGAP_ASN_FILES})

set(NGAP_OAI_generated
  ${NGAP_C_DIR}/ngap_decoder.c
  ${NGAP_C_DIR}/ngap_encoder.c
  ${NGAP_C_DIR}/ngap_xer_print.c
  ${NGAP_C_DIR}/ngap_ies_defs.h
  )

#add_custom_command(
#    OUTPUT ${NGAP_OAI_generated}
#    COMMAND ${asn1c_call} ${NGAP_C_DIR} ${NGAP_ASN_FILES}
#    COMMAND python ${NGAP_DIR}/messages/asn1/asn1tostruct.py -f${NGAP_ASN_DIR}/NGAP-PDU-Contents.asn -o${NGAP_C_DIR}
#    DEPENDS ${NGAP_ASN_FILES}
#)

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

add_executable(${PROJECT_NAME} ${SRCS} )
target_link_libraries(${PROJECT_NAME} pistache pthread)
