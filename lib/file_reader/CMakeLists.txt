add_library(file_reader STATIC ./src/json_reader.cpp)

target_include_directories(
  file_reader
  PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include/
  PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)

target_link_libraries(
  file_reader
  PRIVATE app
  PUBLIC nlohmann_json::nlohmann_json)
