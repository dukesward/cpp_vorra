#include src for unit testing

add_library(vorra_testing main.cpp)

include_directories(
	${CMAKE_SOURCE_DIR}/headers
	${CMAKE_SOURCE_DIR}/headers/network
	${CMAKE_SOURCE_DIR}/src
	${CMAKE_SOURCE_DIR}/src/common
	${CMAKE_SOURCE_DIR}/src/server
	${CMAKE_SOURCE_DIR}/src/server/socket
	${CMAKE_SOURCE_DIR}/src/server/vorra
	${CMAKE_SOURCE_DIR}/src/server/http
)

# include socket headers
file(GLOB SOCKET_SRC
	${CMAKE_SOURCE_DIR}/src/server/socket/*.h
)

add_library(socket_src_test
	${SOCKET_SRC}
)

target_link_libraries(socket_src_test
    common_src
)

set(test_exe main.cpp)
add_executable(vtest ${test_exe})

target_link_libraries(vtest
	socket_src
	vorra_src
	vorra_http
	boost
)