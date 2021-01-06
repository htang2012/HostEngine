cmake_minimum_required(VERSION 3.14.0)
project(habanapb)

#include(FindProtobuf)
find_package(PkgConfig REQUIRED)
pkg_search_module(GRPC REQUIRED grpc)
pkg_search_module(GRPCPP REQUIRED grpc++>1.22.0)


# Find Protobuf installation
# Looks for protobuf-config.cmake file installed by Protobuf's cmake installation.
set(protobuf_MODULE_COMPATIBLE TRUE)
find_package(Protobuf CONFIG REQUIRED)
message(STATUS "Using protobuf ${Protobuf_VERSION}")

set(_PROTOBUF_LIBPROTOBUF protobuf::libprotobuf)
set(_REFLECTION gRPC::grpc++_reflection)
if(CMAKE_CROSSCOMPILING)
  find_program(_PROTOBUF_PROTOC protoc)
else()
  set(_PROTOBUF_PROTOC $<TARGET_FILE:protobuf::protoc>)
endif()


set(PROTO_PATH ${CMAKE_SOURCE_DIR}/protos)
set(HABANA_PROTO "${PROTO_PATH}/habana.proto")
set(GENERATED_PROTOBUF_PATH "${CMAKE_BINARY_DIR}/generated")
message(${CMAKE_BINARY_DIR})
file(MAKE_DIRECTORY ${GENERATED_PROTOBUF_PATH})

set(HABANA_PB_CPP_FILE "${GENERATED_PROTOBUF_PATH}/habana.pb.cc")
set(HABANA_PB_H_FILE "${GENERATED_PROTOBUF_PATH}/habana.pb.h")
set(HABANA_GRPC_PB_CPP_FILE "${GENERATED_PROTOBUF_PATH}/habana.grpc.pb.cc")
set(HABANA_GRPC_PB_H_FILE "${GENERATED_PROTOBUF_PATH}/habana.grpc.pb.h")

add_custom_command( 
    OUTPUT "${HABANA_PB_H_FILE}" 
           "${HABANA_PB_CPP_FILE}" 
           "${HABANA_GRPC_PB_H_FILE}" 
           "${HABANA_GRPC_PB_CPP_FILE}" 
    COMMAND ${PROTOBUF_PROTOC_EXECUTABLE} 
    ARGS "--proto_path=${PROTO_PATH}" 
         "--cpp_out=${GENERATED_PROTOBUF_PATH}" 
         "--grpc_out=${GENERATED_PROTOBUF_PATH}" 
         "--plugin=protoc-gen-grpc=/usr/local/bin/grpc_cpp_plugin" 
         "${HABANA_PROTO}" 
    DEPENDS "${HABANA_PROTO}"
    ) 

include_directories("/home/ytang/.local/include")
SET(${PROJECT_NAME}_PATH_INSTALL "~/HostEngine"   CACHE PATH "This directory contains installation Path")
 
set(GENERATED_PROTOBUF_FILES ${HABANA_PB_H_FILE} ${HABANA_PB_CPP_FILE} 
	${HABANA_GRPC_PB_H_FILE} ${HABANA_GRPC_PB_CPP_FILE}) 

add_library(${PROJECT_NAME} SHARED ${GENERATED_PROTOBUF_FILES})

install(TARGETS ${PROJECT_NAME}
	DESTINATION "${${PROJECT_NAME}_PATH_INSTALL}/lib/")

#add_library(protolibStatic STATIC ${GENERATED_PROTOBUF_FILES})


