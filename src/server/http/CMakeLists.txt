#include src for authentication
include_directories(
	${CMAKE_SOURCE_DIR}/headers
	${CMAKE_SOURCE_DIR}/headers/network
	${CMAKE_SOURCE_DIR}/src/common
	${CMAKE_SOURCE_DIR}/src/server
	${CMAKE_SOURCE_DIR}/src/server/socket
)

add_library(vorra_http
	request_handler.h
	request_handler.cpp
	request.h
	request.cpp
	response.h
	response.cpp
)