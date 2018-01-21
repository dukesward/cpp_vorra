#include src for auth server

add_subdirectory(vorra)
add_subdirectory(http)

add_library(vorra_server main.cpp)

include_directories(
	${CMAKE_SOURCE_DIR}/headers
	${CMAKE_SOURCE_DIR}/headers/network
	${CMAKE_SOURCE_DIR}/src
	${CMAKE_SOURCE_DIR}/src/common
	${CMAKE_SOURCE_DIR}/src/server
	${CMAKE_SOURCE_DIR}/src/server/socket
	${CMAKE_SOURCE_DIR}/src/server/vorra
	${CMAKE_SOURCE_DIR}/src/server/http
	${CMAKE_SOURCE_DIR}/lib
	${CMAKE_SOURCE_DIR}/lib/boost
)

# include socket headers
file(GLOB SOCKET_SRC
	${CMAKE_SOURCE_DIR}/src/server/socket/*.h
)

add_library(socket_src
	${SOCKET_SRC}
)

target_link_libraries(socket_src
    common_src
)

set(server_exe test.h main.cpp)
add_executable(vorra ${server_exe})

target_link_libraries(vorra
	socket_src
	vorra_src
	vorra_http
	boost
)