add_library(server STATIC server.cpp)

target_include_directories(server PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

target_link_libraries(server PUBLIC application Crow::Crow Threads::Threads
                                    ZLIB::ZLIB OpenSSL::SSL OpenSSL::Crypto)
