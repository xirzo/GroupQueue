add_library(sqlite_repository STATIC sqlite_repository.cpp)

target_include_directories(sqlite_repository PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

target_link_libraries(sqlite_repository PUBLIC SQLiteCpp application)
