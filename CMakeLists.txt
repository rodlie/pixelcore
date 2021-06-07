#
# PixelCore
#
# https://pixelcore.org
# https://github.com/rodlie/pixelcore
#
# Copyright (c) 2021 Ole-André Rodlie. All rights reserved.
#
# This program is free software; you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation; either version 3 of the License, or
# (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program.  If not, see <http://www.gnu.org/licenses/>
#

cmake_minimum_required(VERSION 3.5)

project(pixelcore VERSION 0.1.0 LANGUAGES CXX)

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

add_definitions(-DPIXELCORE_VERSION="${PROJECT_VERSION}")

find_package(Qt5 5.12 REQUIRED Core Gui Widgets Concurrent Test)
find_package(PkgConfig REQUIRED)
pkg_search_module(LCMS2 REQUIRED lcms2)

include_directories(
    src
    ${LCMS2_INCLUDE_DIRS}
)

set(PIXELCORE_HEADERS
    src/pixelcorecms.h
    src/pixelcoreimage.h
    src/pixelcoreutils.h
)

set(PIXELCORE_LIBS
    src/pixelcorecms.cpp
    src/pixelcoreimage.cpp
    src/pixelcoreutils.cpp
)

set(PIXELCORE_APP
    src/main.cpp
)

add_executable(${PROJECT_NAME}
    ${PIXELCORE_APP}
    ${PIXELCORE_LIBS}
    ${PIXELCORE_HEADERS}
)

target_link_libraries(${PROJECT_NAME}
    Qt5::Core
    Qt5::Gui
    Qt5::Widgets
    Qt5::Concurrent
    ${LCMS2_LIBRARIES}
)
