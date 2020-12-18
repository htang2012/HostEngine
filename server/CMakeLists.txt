cmake_minimum_required(VERSION 3.14.4)
project(HabanaLabs_HPUServer)



set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(PkgConfig REQUIRED)
find_package(Threads REQUIRED)
set(protobuf_MODULE_COMPATIBLE TRUE)
find_package(Protobuf CONFIG REQUIRED)
message(STATUS "Using protobuf ${Protobuf_VERSION}")
set(_PROTOBUF_LIBPROTOBUF protobuf::libprotobuf)
set(GRPC_REFLECTION gRPC::grpc++_reflection)
if(CMAKE_CROSSCOMPILING)
  find_program(_PROTOBUF_PROTOC protoc)
else()
  set(_PROTOBUF_PROTOC $<TARGET_FILE:protobuf::protoc>)
endif()

# Find gRPC installation
# Looks for gRPCConfig.cmake file installed by gRPC's cmake installation.
find_package(gRPC CONFIG REQUIRED)
message(STATUS "Using gRPC ${gRPC_VERSION}")
pkg_check_modules(GRPC REQUIRED grpc)
pkg_check_modules(GRPCPP REQUIRED grpc++)
message(STATUS "XXXXX")
message(STATUS ${GRPC_INCLUDE_DIR} ${GRPCPP_INCLUDE_DIR})
message(STATUS "XXXXX")

set(_GRPC_GRPCPP gRPC::grpc++)
if(CMAKE_CROSSCOMPILING)
  find_program(_GRPC_CPP_PLUGIN_EXECUTABLE grpc_cpp_plugin)
  message(STATUS "grpc cpp plugin executable")
else()
  set(_GRPC_CPP_PLUGIN_EXECUTABLE $<TARGET_FILE:gRPC::grpc_cpp_plugin>)
endif()

set(PROTO_PATH "${CMAKE_SOURCE_DIR}/protos")
set(HABANA_PROTO "${PROTO_PATH}/habana.proto")
set(GENERATED_PROTOBUF_PATH "${CMAKE_BINARY_DIR}/generated")
file(MAKE_DIRECTORY ${GENERATED_PROTOBUF_PATH})
set(GENERATED_PROTOBUF_LIB "habanapb")

set(GENERATED_PROTOBUF_LIB_PATH "${CMAKE_BINARY_DIR}/protos")
link_directories(${GENERATED_PROTOBUF_LIB_PATH})

#include_directories(${CMAKE_BINARY_DIR}/protos)
include_directories(${GENERATED_PROTOBUF_PATH} ${GRPC_INCLUDE_DIR} ${GRPCPP_INCLUDE_DIR})
#foreach(target HPUserver)
set(target HPUserver)
add_executable(${target} ${CMAKE_CURRENT_SOURCE_DIR}/${target}.cpp)
target_link_libraries(${target} ${GRPC_REFLECTION} ${GRPC_LIBRARIES}  ${GENERATED_PROTOBUF_LIB} ${Protobuf_LIBRARIES})
SET(INSTALL_PATH_INSTALL "~/HostEngine"   CACHE PATH "This directory contains installation Path")
install(TARGETS ${target} 
	ARCHIVE DESTINATION "${INSTALL_PATH_INSTALL}/lib/"
	LIBRARY DESTINATION "${INSTALL_PATH_INSTALL}/lib/"
	RUNTIME DESTINATION "${INSTALL_PATH_INSTALL}/lib/"
	INCLUDES DESTINATION "${INSTALL_PATH_INSTALL}/include/")


#endforeach()
