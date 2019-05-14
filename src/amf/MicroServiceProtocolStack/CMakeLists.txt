cmake_minimum_required (VERSION 3.2)

project(amf-server)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -pg -g3" )

link_directories(/usr/local/lib/)

aux_source_directory(CommService/model COMM_MODEL_SOURCES)
aux_source_directory(EvetService/model EVET_MODEL_SOURCES)
aux_source_directory(LocService/model LOC_MODEL_SOURCES)
aux_source_directory(MtService/model MT_MODEL_SOURCES)
file(GLOB AMF_API_SOURCES
"CommService/api/AMFStatusChangeSubscribeApi.h"
"CommService/api/AMFStatusChangeSubscribeApi.cpp"
"CommService/api/AMFStatusChangeSubscribeModifyApi.h"
"CommService/api/AMFStatusChangeSubscribeModifyApi.cpp"
"CommService/api/AMFStatusChangeUnSubscribeApi.h"
"CommService/api/AMFStatusChangeUnSubscribeApi.cpp"
"CommService/api/CreateUEContxtApi.h"
"CommService/api/CreateUEContxtApi.cpp"
"CommService/api/EBIAssignmentApi.h"
"CommService/api/EBIAssignmentApi.cpp"
"CommService/api/N1N2MessageSubscribeApi.h"
"CommService/api/N1N2MessageSubscribeApi.cpp"
"CommService/api/N1N2MessageTransferApi.h"
"CommService/api/N1N2MessageTransferApi.cpp"
"CommService/api/N1N2MessageUnSubscribeApi.h"
"CommService/api/N1N2MessageUnSubscribeApi.cpp"
"CommService/api/NonUEN2InfoSubscribeApi.h"
"CommService/api/NonUEN2InfoSubscribeApi.cpp"
"CommService/api/NonUEN2InfoUnSubscribeApi.h"
"CommService/api/NonUEN2InfoUnSubscribeApi.cpp"
"CommService/api/NonUEN2MessageTransferApi.h"
"CommService/api/NonUEN2MessageTransferApi.cpp"
"CommService/api/ReleaseUEContxtApi.h"
"CommService/api/ReleaseUEContxtApi.cpp"
"CommService/api/UEContextTransferApi.h"
"CommService/api/UEContextTransferApi.cpp"


"CommService/impl/AMFStatusChangeSubscribeApiImpl.h"
"CommService/impl/AMFStatusChangeSubscribeApiImpl.cpp"
"CommService/impl/AMFStatusChangeSubscribeModifyApiImpl.h"
"CommService/impl/AMFStatusChangeSubscribeModifyApiImpl.cpp"
"CommService/impl/AMFStatusChangeUnSubscribeApiImpl.h"
"CommService/impl/AMFStatusChangeUnSubscribeApiImpl.cpp"
"CommService/impl/CreateUEContxtApiImpl.h"
"CommService/impl/CreateUEContxtApiImpl.cpp"
"CommService/impl/EBIAssignmentApiImpl.h"
"CommService/impl/EBIAssignmentApiImpl.cpp"
"CommService/impl/N1N2MessageSubscribeApiImpl.h"
"CommService/impl/N1N2MessageSubscribeApiImpl.cpp"
"CommService/impl/N1N2MessageTransferApiImpl.h"
"CommService/impl/N1N2MessageTransferApiImpl.cpp"
"CommService/impl/N1N2MessageUnSubscribeApiImpl.h"
"CommService/impl/N1N2MessageUnSubscribeApiImpl.cpp"
"CommService/impl/NonUEN2InfoSubscribeApiImpl.h"
"CommService/impl/NonUEN2InfoSubscribeApiImpl.cpp"
"CommService/impl/NonUEN2InfoUnSubscribeApiImpl.h"
"CommService/impl/NonUEN2InfoUnSubscribeApiImpl.cpp"
"CommService/impl/NonUEN2MessageTransferApiImpl.h"
"CommService/impl/NonUEN2MessageTransferApiImpl.cpp"
"CommService/impl/ReleaseUEContxtApiImpl.h"
"CommService/impl/ReleaseUEContxtApiImpl.cpp"
"CommService/impl/UEContextTransferApiImpl.h"
"CommService/impl/UEContextTransferApiImpl.cpp"



"EvetService/api/CreateSubscriptionApi.h"
"EvetService/api/DeleteSubscriptionApi.h"
"EvetService/api/ModifySubscriptionApi.h"
"EvetService/api/CreateSubscriptionApi.cpp"
"EvetService/api/DeleteSubscriptionApi.cpp"
"EvetService/api/ModifySubscriptionApi.cpp"

"EvetService/impl/CreateSubscriptionApiImpl.h"
"EvetService/impl/DeleteSubscriptionApiImpl.h"
"EvetService/impl/ModifySubscriptionApiImpl.h"
"EvetService/impl/CreateSubscriptionApiImpl.cpp"
"EvetService/impl/DeleteSubscriptionApiImpl.cpp"
"EvetService/impl/ModifySubscriptionApiImpl.cpp"


"ProvideLocationApi.h"
"LocService/api/ProvideLocationApi.cpp"
"LocService/impl/ProvideLocationApiImpl.h"
"LocService/impl/ProvideLocationApiImpl.cpp"

"MtService/api/EnableUEReachabilityApi.h"
"MtService/api/EnableUEReachabilityApi.cpp"
"MtService/api/ProvideDomainSelectionInfoApi.h"
"MtService/api/ProvideDomainSelectionInfoApi.cpp"

"MtService/impl/EnableUEReachabilityApiImpl.h"
"MtService/impl/EnableUEReachabilityApiImpl.cpp"
"MtService/impl/ProvideDomainSelectionInfoApiImpl.h"
"MtService/impl/ProvideDomainSelectionInfoApiImpl.cpp"
)

include_directories(CommService/model)
include_directories(CommService/api)
include_directories(CommService/impl)
include_directories(EvetService/model)
include_directories(EvetService/api)
include_directories(EvetService/impl)
include_directories(LocService/model)
include_directories(LocService/api)
include_directories(LocService/impl)
include_directories(MtService/model)
include_directories(MtService/api)
include_directories(MtService/impl)

set(AMF_API_SERVER_SOURCES
amf-services.cpp
    ${COMM_MODEL_SOURCES}
    ${EVET_MODEL_SOURCES}
    ${LOC_MODEL_SOURCES}
    ${MT_MODEL_SOURCES}
    ${AMF_API_SOURCES})

add_executable(${PROJECT_NAME} ${AMF_API_SERVER_SOURCES} )
target_link_libraries(${PROJECT_NAME} pistache pthread)
