add_library(file_reader STATIC json_reader.cpp)

target_include_directories(file_reader
                           PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)

target_link_libraries(
  file_reader
  PRIVATE domain
  PUBLIC nlohmann_json::nlohmann_json)
