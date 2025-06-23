add_library(db STATIC db.c)

target_include_directories(db PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

target_link_libraries(db PUBLIC pq logger)


add_library(server STATIC server.c)

target_include_directories(server PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

target_link_libraries(server PUBLIC db event logger)
