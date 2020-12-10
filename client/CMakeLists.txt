cmake_minimum_required(VERSION 3.14.4)
project(HabanaLabs_HPUClient)

set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Protobuf REQUIRED)
find_package(Threads REQUIRED)
#find_package(gRPC CONFIG REQUIRED)
if (gRPC_FOUND)
   Message("Find grpc package")
   set(GRPC_REFLECTION gRPC::grpc++_reflection)
else()
   Message("Not find grpc package")
   set(GRPC_REFLECTION grpc++_reflection)
endif()

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
include_directories(/usr/local/include  /include)
link_directories(/usr/local/lib /lib)

set(PROTO_PATH "${CMAKE_SOURCE_DIR}/protos")
set(HABANA_PROTO "${PROTO_PATH}/habana.proto")
set(GENERATED_PROTOBUF_PATH "${CMAKE_BINARY_DIR}/generated")
file(MAKE_DIRECTORY ${GENERATED_PROTOBUF_PATH})
set(GENERATED_PROTOBUF_LIB "habanapb")

set(GENERATED_PROTOBUF_LIB_PATH "${CMAKE_BINARY_DIR}/protos")
link_directories(${GENERATED_PROTOBUF_LIB_PATH})

#include_directories(${CMAKE_BINARY_DIR}/protos)
include_directories(${GENERATED_PROTOBUF_PATH})
#foreach(target HPUclient)
set(target HPUclient)
add_executable(${target} ${CMAKE_CURRENT_SOURCE_DIR}/${target}.cpp)
target_link_libraries(${target} ${GRPC_REFLECTION} grpc++  ${GENERATED_PROTOBUF_LIB})
SET(INSTALL_PATH_INSTALL "~/HostEngine"   CACHE PATH "This directory contains installation Path")
install(TARGETS ${target} 
	ARCHIVE DESTINATION "${INSTALL_PATH_INSTALL}/lib/"
	LIBRARY DESTINATION "${INSTALL_PATH_INSTALL}/lib/"
	RUNTIME DESTINATION "${INSTALL_PATH_INSTALL}/lib/"
	INCLUDES DESTINATION "${INSTALL_PATH_INSTALL}/include/")


#endforeach()
