#
# PIXELCORE
#
# https://pixelcore.org
# https://github.com/rodlie/pixelcore
#
# Copyright (c) Ole-André Rodlie. All rights reserved.
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
# The copyright holders of PIXELCORE hereby grant permission for
# non-GPL compatible plug-ins and add-ons to be used and distributed
# together with PIXELCORE, provided that you also meet the terms and conditions
# of the licenses of those plug-ins and add-ons.
#

cmake_minimum_required(VERSION 3.5)

project(pixelcore VERSION 0.1.0 LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if (CMAKE_BUILD_TYPE MATCHES "^(release|Release|RELEASE)$")
    add_definitions(-DQT_NO_DEBUG_OUTPUT)
else()
    add_definitions(-DQT_MESSAGELOGCONTEXT -DPIXELCORE_DEBUG)
endif()

add_definitions(-DPIXELCORE_VERSION="${PROJECT_VERSION}")

# Qt
find_package(QT NAMES Qt6 Qt5 COMPONENTS Core REQUIRED)
find_package(Qt${QT_VERSION_MAJOR} 5.12 COMPONENTS Gui Widgets Concurrent Test REQUIRED)

# pkg-config
find_package(PkgConfig REQUIRED)

# Litte CMS
pkg_search_module(LCMS2 2.13 REQUIRED lcms2)
link_directories(${LCMS2_LIBDIR})

# OpenImageIO
pkg_search_module(OIIO 2.1 REQUIRED OpenImageIO)
link_directories(${OIIO_LIBDIR})

include_directories(
    core
    ${LCMS2_INCLUDE_DIRS}
    ${OIIO_INCLUDE_DIRS}
)

set(PIXELCORE_HEADERS_CORE
    core/cms.h
    core/console.h
)

set(PIXELCORE_SOURCES_CORE
    core/cms.cpp
    core/console.cpp
)

set(PIXELCORE_APP_CORE
    core/main.cpp
)

add_executable(
    ${PROJECT_NAME}
    ${PIXELCORE_APP_CORE}
    ${PIXELCORE_SOURCES_CORE}
    ${PIXELCORE_HEADERS_CORE}
)

target_link_libraries(
    ${PROJECT_NAME}
    PRIVATE
    Qt${QT_VERSION_MAJOR}::Core
    Qt${QT_VERSION_MAJOR}::Gui
    Qt${QT_VERSION_MAJOR}::Widgets
    ${LCMS2_LIBRARIES}
    ${OIIO_LIBRARIES}
)
