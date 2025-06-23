add_library(db STATIC db.c)

target_include_directories(db PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

target_link_libraries(db PUBLIC pq)
