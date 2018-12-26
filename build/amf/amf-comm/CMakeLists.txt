cmake_minimum_required (VERSION 3.2)

project(amf-comm)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -pg -g3" )

#include(ExternalProject)

#set(EXTERNAL_INSTALL_LOCATION ${CMAKE_CURRENT_SOURCE_DIR}/external)

#ExternalProject_Add(PISTACHE
#    GIT_REPOSITORY https://github.com/oktal/pistache.git
#    CMAKE_ARGS -DCMAKE_INSTALL_PREFIX=${EXTERNAL_INSTALL_LOCATION}
#)

#ExternalProject_Add(NLOHMANN
#    GIT_REPOSITORY https://github.com/nlohmann/json.git
#    CMAKE_ARGS -DCMAKE_INSTALL_PREFIX=${EXTERNAL_INSTALL_LOCATION}
#)
#include_directories(${EXTERNAL_INSTALL_LOCATION}/include)
include_directories(/usr/local/include/nlohmann)
include_directories(/usr/local/include/pistache)
#include_directories(${CMAKE_CURRENT_SOURCE_DIR}/api)
#include_directories(${CMAKE_CURRENT_SOURCE_DIR}/impl)
#include_directories(${CMAKE_CURRENT_SOURCE_DIR}/model)
#include_directories(${EXTERNAL_INSTALL_LOCATION}/include/nlohmann)
#include_directories(${EXTERNAL_INSTALL_LOCATION}/include/nlohmann)
#include_directories(${EXTERNAL_INSTALL_LOCATION}/include/nlohmann)
#include_directories(${EXTERNAL_INSTALL_LOCATION}/include/nlohmann)
#include_directories(${EXTERNAL_INSTALL_LOCATION}/include/nlohmann)
#include_directories(${EXTERNAL_INSTALL_LOCATION}/include/nlohmann)
link_directories(/usr/local/lib)
#link_directories(${EXTERNAL_INSTALL_LOCATION}/lib)


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
#add_dependencies(${PROJECT_NAME} PISTACHE NLOHMANN)
target_link_libraries(${PROJECT_NAME} pistache pthread)
