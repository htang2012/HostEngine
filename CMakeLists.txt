# Project
project(HabanaLabsHostEngine)
# Minimum CMake required
cmake_minimum_required(VERSION 3.14.4)
add_compile_options(-std=c++11)
find_program(GRPC_CPP_PLUGIN_EXECUTABLE grpc_cpp_plugin)
get_filename_component(HABANA_PROTO "protos/habana.proto" ABSOLUTE)
get_filename_component(HABANA_PROTO_PATH "${HABANA_PROTO}" PATH)
set(HABANA_GRPC_SRCS "${CMAKE_CURRENT_BINARY_DIR}/habana.grpc.pb.cc")
set(HABANA_GRPC_HEADS "${CMAKE_CURRENT_BINARY_DIR}/habana.grpc.pb.h")
set(HABANA_PROTOBUF_SRCS "${CMAKE_CURRENT_BINARY_DIR}/habana.pb.cc")
set(HABANA_PROTOBUF_HEADS "${CMAKE_CURRENT_BINARY_DIR}/habana.pb.h")
set(PROTOC protoc)
set(GRPC_CPP_PLUGIN_PATH "which ${GRPC_CPP_PLUGIN}")

add_custom_command(
    OUTPUT "${HABANA_GRPC_SRCS}" "${HABANA_GRPC_HEADS}" "${HABANA_PROTOBUF_SRCS}" "${HABANA_PROTOBUF_HEADS}"
    COMMAND ${PROTOC}
    ARGS  --grpc_out="${CMAKE_CURRENT_BINARY_DIR}"
    --cpp_out="${CMAKE_CURRENT_BINARY_DIR}"
    -I "${HABANA_PROTO_PATH}"
    --plugin=protoc-gen-grpc="${GRPC_CPP_PLUGIN_EXECUTABLE}"
    "${HABANA_PROTO}" 
    DEPENDS "${HABANA_PROTO}" )

include_directories("${CMAKE_CURRENT_BINARY_DIR}")

foreach(target HPUclient HPUserver )
   add_executable(${target} "${CMAKE_CURRENT_SOURCE_DIR}/src/${target}.cpp" ${HABANA_GRPC_SRCS} ${HABANA_GRPC_HEADS})
   target_link_libraries(${target} )
endforeach()


