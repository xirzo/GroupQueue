add_library(server STATIC server.cpp)

target_include_directories(server PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include/)

target_link_libraries(
  server PUBLIC io Threads::Threads ZLIB::ZLIB OpenSSL::SSL OpenSSL::Crypto
                nlohmann_json::nlohmann_json)
