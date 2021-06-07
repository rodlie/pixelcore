cmake_minimum_required(VERSION 3.5)

project(pixelcore LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if (CMAKE_BUILD_TYPE MATCHES "^(release|Release|RELEASE)$")
    add_definitions(-DQT_NO_DEBUG_OUTPUT)
else()
    add_definitions(-DQT_MESSAGELOGCONTEXT)
endif()

find_package(Qt5 5.12 REQUIRED Core Gui)
find_package(PkgConfig REQUIRED)
pkg_search_module(LCMS2 REQUIRED lcms2)

include_directories(
    libs
    app
    ${LCMS2_INCLUDE_DIRS}
)

set(PIXELCORE_HEADERS
    libs/pixelcorecms.h
    libs/pixelcoreimage.h
)

set(PIXELCORE_LIBS
    libs/pixelcorecms.cpp
    libs/pixelcoreimage.cpp
)

set(PIXELCORE_APP
    app/main.cpp
)

add_executable(${PROJECT_NAME}
    ${PIXELCORE_APP}
    ${PIXELCORE_LIBS}
    ${PIXELCORE_HEADERS}
)

target_link_libraries(${PROJECT_NAME}
    Qt5::Core
    Qt5::Gui
    ${LCMS2_LIBRARIES}
)
