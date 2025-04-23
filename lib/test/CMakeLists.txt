add_library(test_lib ./src/test.cpp)

target_include_directories(test_lib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include/)

add_library(test_lib_headers INTERFACE)

target_include_directories(
  test_lib_headers
  INTERFACE $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
            $<INSTALL_INTERFACE:include>)

target_include_directories(
  test_lib
  PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}
  PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>)

target_link_libraries(test_lib PUBLIC test_lib_headers)
