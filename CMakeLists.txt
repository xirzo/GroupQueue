cmake_minimum_required(VERSION 3.24.4)

project(group_queue LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)

find_package(nlohmann_json REQUIRED)
find_package(PostgreSQL REQUIRED)
find_package(libpqxx REQUIRED)
find_package(Threads REQUIRED)
find_package(ZLIB REQUIRED)
find_package(OpenSSL REQUIRED)
find_package(Crow REQUIRED)

add_subdirectory(bin)
add_subdirectory(lib)

option(BUILD_TESTS "Build tests" OFF)

if(BUILD_TESTS)
  enable_testing()
  add_subdirectory(tests)
endif()
