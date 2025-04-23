add_library(app STATIC ./src/app.cpp ./src/db.cpp)

target_include_directories(
  app
  PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include/
  PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)

target_link_libraries(app PUBLIC SQLiteCpp)
