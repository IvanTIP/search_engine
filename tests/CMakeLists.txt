cmake_minimum_required(VERSION 3.5)

project(test_engine)

include(FetchContent)

set(ARG_DOWNLOAD_EXTRACT_TIMESTAMP TRUE)
FetchContent_Declare(
        googletest
        URL https://github.com/google/googletest/archive/609281088cfefc76f9d0ce82e1ff6c30cc3591e5.zip
        )

FetchContent_MakeAvailable(googletest)

enable_testing()

add_executable(test_engine indexTest.cpp serverTest.cpp
        ${CMAKE_SOURCE_DIR}/src/InvertedIndex.cpp
        ${CMAKE_SOURCE_DIR}/src/SearchServer.cpp)

target_include_directories(test_engine PUBLIC ${gtest_SOURSE_DIR}/include ${MY_INCLUDE_DIR})

target_link_libraries(test_engine PRIVATE gtest_main)

