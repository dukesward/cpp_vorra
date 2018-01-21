#include src for common lib
include_directories(
	${CMAKE_SOURCE_DIR}/headers
	${CMAKE_SOURCE_DIR}/headers/network
)

# include headers
file(GLOB GLOBAL_H
	${CMAKE_SOURCE_DIR}/headers/define.h
	${CMAKE_SOURCE_DIR}/headers/network/*.h
)

add_library(common_src
	${GLOBAL_H}
	log.h
	log.cpp
	numeric.h
	data_struct.h
	c_list.h
	c_list.cpp
	string_utils.h
	message_buf.h
)