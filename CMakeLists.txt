cmake_minimum_required(VERSION 3.14.0)
project(Habana_HE VERSION 0.1.0)
SET(${PROJ_NAME}_PATH_INSTALL "~/HostEngine"   CACHE PATH "This directory contains installation Path")
add_subdirectory(protos)
add_subdirectory(client)
add_subdirectory(server)

install(TARGETS ${PROJ_NAME} 
	ARCHIVE DESTINATION "${${PROJ_NAME}}/lib"
	ARCHIVE DESTINATION "${${PROJ_NAME}}/exe")



