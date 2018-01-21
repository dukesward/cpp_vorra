# add common source files
add_subdirectory(common)

# add server source files according to options
if(SERVERS)
	add_subdirectory(server)
endif()