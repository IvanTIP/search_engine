cmake_minimum_required(VERSION 3.5)

set(ARG_DOWNLOAD_EXTRACT_TIMESTAMP TRUE)
include(FetchContent)
FetchContent_Declare(
        json
        URL  https://github.com/nlohmann/json/releases/download/v3.11.3/json.tar.xz
        )

FetchContent_MakeAvailable(json)

set(PROJECT_SOURCES
        main.cpp
        InvertedIndex.cpp
        convertorJSON.cpp
        SearchServer.cpp
)

if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(search_engine
        MANUAL_FINALIZATION
        ${PROJECT_SOURCES}
    )

else()
    if(ANDROID)
        add_library(search_engine SHARED
            ${PROJECT_SOURCES}
        )


    else()
        add_executable(search_engine
            ${PROJECT_SOURCES}
        )
    endif()
endif()

target_include_directories(search_engine PUBLIC ${MY_INCLUDE_DIR})

target_link_libraries(search_engine PRIVATE nlohmann_json::nlohmann_json Qt${QT_VERSION_MAJOR}::Widgets)

set_target_properties(search_engine PROPERTIES
    MACOSX_BUNDLE_GUI_IDENTIFIER my.example.com
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

install(TARGETS search_engine
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR})

if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(search_engine)
endif()
