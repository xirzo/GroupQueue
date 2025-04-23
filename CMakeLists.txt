cmake_minimum_required(VERSION 3.30.5)

project(group_queue LANGUAGES CXX)

set(CMAKE_CXX_STANDARD_REQUIRED True)

find_package(nlohmann_json 3.11.3 REQUIRED)

add_subdirectory(bin)
add_subdirectory(lib)

option(BUILD_TESTS "Build tests" OFF)

if(BUILD_TESTS)
  enable_testing()
  add_subdirectory(tests)
endif()
