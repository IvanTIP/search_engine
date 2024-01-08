cmake_minimum_required(VERSION 3.20)
project(search_engine VERSION 1.0)

set(MY_INCLUDE_DIR ${CMAKE_SOURCE_DIR}/include)

add_subdirectory(src)
add_subdirectory(tests)