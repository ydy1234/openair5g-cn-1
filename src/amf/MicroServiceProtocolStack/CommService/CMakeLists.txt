cmake_minimum_required (VERSION 3.2)

project(comm-api-server)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -pg -g3" )

link_directories(/usr/local/lib/)

aux_source_directory(model MODEL_SOURCES)
file(GLOB COMM_API_SOURCES
"api/AMFStatusChangeSubscribeApi.h"
"api/AMFStatusChangeSubscribeApi.cpp"
"api/AMFStatusChangeSubscribeModifyApi.h"
"api/AMFStatusChangeSubscribeModifyApi.cpp"
"api/AMFStatusChangeUnSubscribeApi.h"
"api/AMFStatusChangeUnSubscribeApi.cpp"
"api/CreateUEContxtApi.h"
"api/CreateUEContxtApi.cpp"
"api/EBIAssignmentApi.h"
"api/EBIAssignmentApi.cpp"
"api/N1N2MessageSubscribeApi.h"
"api/N1N2MessageSubscribeApi.cpp"
"api/N1N2MessageTransferApi.h"
"api/N1N2MessageTransferApi.cpp"
"api/N1N2MessageUnSubscribeApi.h"
"api/N1N2MessageUnSubscribeApi.cpp"
"api/NonUEN2InfoSubscribeApi.h"
"api/NonUEN2InfoSubscribeApi.cpp"
"api/NonUEN2InfoUnSubscribeApi.h"
"api/NonUEN2InfoUnSubscribeApi.cpp"
"api/NonUEN2MessageTransferApi.h"
"api/NonUEN2MessageTransferApi.cpp"
"api/ReleaseUEContxtApi.h"
"api/ReleaseUEContxtApi.cpp"
"api/UEContextTransferApi.h"
"api/UEContextTransferApi.cpp"


"impl/AMFStatusChangeSubscribeApiImpl.h"
"impl/AMFStatusChangeSubscribeApiImpl.cpp"
"impl/AMFStatusChangeSubscribeModifyApiImpl.h"
"impl/AMFStatusChangeSubscribeModifyApiImpl.cpp"
"impl/AMFStatusChangeUnSubscribeApiImpl.h"
"impl/AMFStatusChangeUnSubscribeApiImpl.cpp"
"impl/CreateUEContxtApiImpl.h"
"impl/CreateUEContxtApiImpl.cpp"
"impl/EBIAssignmentApiImpl.h"
"impl/EBIAssignmentApiImpl.cpp"
"impl/N1N2MessageSubscribeApiImpl.h"
"impl/N1N2MessageSubscribeApiImpl.cpp"
"impl/N1N2MessageTransferApiImpl.h"
"impl/N1N2MessageTransferApiImpl.cpp"
"impl/N1N2MessageUnSubscribeApiImpl.h"
"impl/N1N2MessageUnSubscribeApiImpl.cpp"
"impl/NonUEN2InfoSubscribeApiImpl.h"
"impl/NonUEN2InfoSubscribeApiImpl.cpp"
"impl/NonUEN2InfoUnSubscribeApiImpl.h"
"impl/NonUEN2InfoUnSubscribeApiImpl.cpp"
"impl/NonUEN2MessageTransferApiImpl.h"
"impl/NonUEN2MessageTransferApiImpl.cpp"
"impl/ReleaseUEContxtApiImpl.h"
"impl/ReleaseUEContxtApiImpl.cpp"
"impl/UEContextTransferApiImpl.h"
"impl/UEContextTransferApiImpl.cpp"
)

include_directories(model)
include_directories(api)
include_directories(impl)

set(COMM_API_SERVER_SOURCES
comm-service.cpp
    ${MODEL_SOURCES}
    ${COMM_API_SOURCES})

add_executable(${PROJECT_NAME} ${COMM_API_SERVER_SOURCES} )
target_link_libraries(${PROJECT_NAME} pistache pthread)
