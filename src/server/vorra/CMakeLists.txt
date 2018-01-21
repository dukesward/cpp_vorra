#include src for authentication
include_directories(
	${CMAKE_SOURCE_DIR}/headers
	${CMAKE_SOURCE_DIR}/headers/network
	${CMAKE_SOURCE_DIR}/src/common
	${CMAKE_SOURCE_DIR}/src/server
	${CMAKE_SOURCE_DIR}/src/server/socket
	${CMAKE_SOURCE_DIR}/src/server/http
)

add_library(vorra_src
	core.h
	core.cpp
)