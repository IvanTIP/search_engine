cmake_minimum_required(VERSION 3.5)


project(search_engine VERSION 1.0 LANGUAGES CXX)


set(MY_INCLUDE_DIR ${CMAKE_SOURCE_DIR}/include)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets)


add_subdirectory(src)

add_subdirectory(tests)



