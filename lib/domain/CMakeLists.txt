add_library(domain STATIC ./list_holder.cpp)

target_include_directories(domain PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)

target_link_libraries(domain PUBLIC SQLiteCpp)
