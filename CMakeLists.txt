cmake_minimum_required(VERSION 3.14.0)
project(Habana_HE VERSION 0.1.0)
find_package(PkgConfig REQUIRED)

#the following should be replaced with:
# cmake -DCMAKE_INSTALL_PREFIX=/home/ytang/.local ..
include_directories("/home/ytang/.local/include")
link_directories("/home/ytang/.local/lib")
SET(${PROJ_NAME}_PATH_INSTALL "~/HostEngine"   CACHE PATH "This directory contains installation Path")
add_subdirectory(protos)
add_subdirectory(client)
add_subdirectory(server)

install(TARGETS ${PROJ_NAME} 
	ARCHIVE DESTINATION "${${PROJ_NAME}}/lib"
	ARCHIVE DESTINATION "${${PROJ_NAME}}/exe")



