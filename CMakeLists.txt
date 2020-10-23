cmake_minimum_required(VERSION 3.14.4)
project(HabanaLabs_HPU)

set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Protobuf REQUIRED)
find_package(Threads REQUIRED)
find_package(gRPC CONFIG REQUIRED)
if (gRPC_FOUND)
   Message("Find grpc package")
   set(GRPC_REFLECTION gRPC::grpc++_reflection)
else()
   Message("Not find grpc package")
   set(GRPC_REFLECTION grpc++_reflection)
endif()

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
include_directories(/usr/local/include)
link_directories(/usr/local/lib)

set(PROTO_PATH "${CMAKE_SOURCE_DIR}/protos")
set(HABANA_PROTO "${PROTO_PATH}/habana.proto")
set(GENERATED_PROTOBUF_PATH "${CMAKE_BINARY_DIR}/generated")
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

include_directories(${GENERATED_PROTOBUF_PATH})
foreach(target HPUclient HPUserver)
    add_executable(${target} ${CMAKE_CURRENT_SOURCE_DIR}/src/${target}.cpp ${GENERATED_PROTOBUF_FILES})
    target_link_libraries(${target} ${GRPC_REFLECTION} grpc++  ${PROTOBUF_LIBRARY})
endforeach()
