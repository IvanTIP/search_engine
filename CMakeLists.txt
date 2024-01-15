cmake_minimum_required(VERSION 3.20)

project(search_engine VERSION 1.0)

set(MY_INCLUDE_DIR ${CMAKE_SOURCE_DIR}/include)
set(CMAKE_AUTOMOC ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)
find_package(Qt5Widgets REQUIRED)


include_directories(${Qt5Widgets_INCLUDES})
add_definitions(${Qt5Widgets_DEFINITIONS})
set(CMAKE_CXX_FLAGS "${Qt5Widgets_EXECUTABLE_COMPILE_FLAGS}")
add_subdirectory(src)

add_subdirectory(tests)



