add_library(list_holder STATIC list_holder.cpp)

target_include_directories(list_holder PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

target_link_libraries(list_holder PUBLIC SQLiteCpp model)
