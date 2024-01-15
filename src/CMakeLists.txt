cmake_minimum_required(VERSION 3.20)

include(FetchContent)
FetchContent_Declare(
        json
        URL  https://github.com/nlohmann/json/releases/download/v3.11.3/json.tar.xz
        )

FetchContent_MakeAvailable(json)

add_executable(search_engine main.cpp InvertedIndex.cpp convertorJSON.cpp SearchServer.cpp)

target_include_directories(search_engine PUBLIC ${MY_INCLUDE_DIR})

target_link_libraries(search_engine PRIVATE nlohmann_json::nlohmann_json ${Qt5Widgets_LIBRARIES})