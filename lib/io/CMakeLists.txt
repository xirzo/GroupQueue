add_subdirectory(server)
add_subdirectory(command)

add_library(io INTERFACE)

target_include_directories(io INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/include/)
