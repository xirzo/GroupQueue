add_library(postgre postgre.cpp)

target_include_directories(postgre PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

target_link_libraries(postgre PUBLIC pqxx pq application)
