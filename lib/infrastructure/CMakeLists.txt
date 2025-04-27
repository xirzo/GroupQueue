add_library(infrastructure STATIC server.cpp)

target_include_directories(infrastructure PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

target_link_libraries(
  infrastructure
  PUBLIC domain
         application
         Crow::Crow
         nlohmann_json::nlohmann_json
         Threads::Threads
         ZLIB::ZLIB
         OpenSSL::SSL
         OpenSSL::Crypto)
