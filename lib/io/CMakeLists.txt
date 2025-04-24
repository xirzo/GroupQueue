add_subdirectory(server)

add_library(io INTERFACE)

target_include_directories(server PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include/)
