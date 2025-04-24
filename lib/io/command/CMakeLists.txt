add_library(command STATIC post_list.cpp)

target_include_directories(command PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include/)

target_link_libraries(command PUBLIC io domain)
