add_library(application INTERFACE)

target_include_directories(application INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})

target_link_libraries(application INTERFACE domain)
