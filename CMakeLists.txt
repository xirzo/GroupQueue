cmake_minimum_required(VERSION 3.24.4)

project(group_queue LANGUAGES C)

add_subdirectory(bin)
add_subdirectory(src)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_EXTENSIONS OFF)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON CACHE INTERNAL "")
set(CMAKE_PREFIX_PATH /usr/lib/ /usr/local/lib/)

file(CREATE_LINK "${CMAKE_BINARY_DIR}/compile_commands.json"
                 "${CMAKE_SOURCE_DIR}/compile_commands.json"
                 COPY_ON_ERROR)

find_library(LIBPQ NAMES pq)
find_library(LOGGER NAMES logger)

if (NOT LIBPQ)
    message("libpq not found!")
endif()

if (NOT LOGGER)
    message("logger not found!")
endif()
