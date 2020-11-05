cmake_minimum_required(VERSION 3.14.0)
project(habanapb)

include(FindProtobuf)
find_package(Protobuf REQUIRED)

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
         "${HABANA_PROTO}" 
    COMMAND ${PROTOBUF_PROTOC_EXECUTABLE} 
    ARGS "--proto_path=${PROTO_PATH}" 
         "--grpc_out=${GENERATED_PROTOBUF_PATH}" 
         "--plugin=protoc-gen-grpc=/usr/local/bin/grpc_cpp_plugin" 
         "${HABANA_PROTO}" 
    ) 
 
set(GENERATED_PROTOBUF_FILES ${HABANA_PB_H_FILE} ${HABANA_PB_CPP_FILE} 
    ${HABANA_GRPC_PB_H_FILE} ${HABANA_GRPC_PB_CPP_FILE}) 

add_library(${PROJECT_NAME} SHARED ${GENERATED_PROTOBUF_FILES})

#add_library(protolibStatic STATIC ${GENERATED_PROTOBUF_FILES})


