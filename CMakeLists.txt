cmake_minimum_required(VERSION 3.15)
project(h264WebSocketServer)

set(CMAKE_CXX_STANDARD 14)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOMOC ON)

find_package(Qt5 COMPONENTS Core Network WebSockets REQUIRED)

add_library(h264ServiceLib INTERFACE)
set_target_properties(h264ServiceLib PROPERTIES
        INTERFACE_INCLUDE_DIRECTORIES ${CMAKE_SOURCE_DIR}/src)
target_link_libraries(h264ServiceLib INTERFACE libavformat libavcodec Qt5::Core Qt5::WebSockets)

add_executable(h264WebSocketServer main.cpp)
target_link_libraries(h264WebSocketServer PRIVATE libavformat h264ServiceLib Qt5::Core)

include(ExternalProject)
ExternalProject_Add(ffmpeg
        SVN_REPOSITORY https://192.168.15.10:8443/svn/team2.repo/ffmpeg-for-stream/trunk
        CONFIGURE_COMMAND ""
        BUILD_COMMAND ""
        INSTALL_COMMAND "")
ExternalProject_Get_Property(ffmpeg SOURCE_DIR)
#ExternalProject_Get_Property(ffmpeg BINARY_DIR)
#message(STATUS "source_dir=${SOURCE_DIR}")
#message(STATUS "binary_dir=${BINARY_DIR}")

###############
# libavformat #
###############
add_library(libavformat SHARED IMPORTED)
set_target_properties(libavformat PROPERTIES
        IMPORTED_LOCATION ${SOURCE_DIR}/bin/avformat-58.dll
        IMPORTED_IMPLIB ${SOURCE_DIR}/lib/avformat.lib
        INTERFACE_INCLUDE_DIRECTORIES ${SOURCE_DIR}/include)
add_dependencies(libavformat ffmpeg)

##############
# libavcodec #
##############
add_library(libavcodec SHARED IMPORTED)
set_target_properties(libavcodec PROPERTIES
        IMPORTED_LOCATION ${SOURCE_DIR}/bin/avcodec-58.dll
        IMPORTED_IMPLIB ${SOURCE_DIR}/lib/avcodec.lib
        INTERFACE_INCLUDE_DIRECTORIES ${SOURCE_DIR}/include)
add_dependencies(libavcodec ffmpeg)


add_subdirectory(tests)