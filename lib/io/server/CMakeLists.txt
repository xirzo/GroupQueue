add_library(server STATIC server.cpp)

target_include_directories(
  server
  PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include/
  PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include/)

target_link_libraries(server PUBLIC app Threads::Threads ZLIB::ZLIB
                                    OpenSSL::SSL OpenSSL::Crypto)
