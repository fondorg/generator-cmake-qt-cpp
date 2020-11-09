cmake_minimum_required(VERSION 3.17)
project(<%= project %>)

set(CMAKE_CXX_STANDARD 17)

if (NOT DEFINED ENV{QT5_PREFIX})
    message(FATAL_ERROR "QT5_PREFIX environment variable is not set")
endif ()

<% if (opencv) { %>
if (NOT DEFINED ENV{OPENCV_DIR})
    message(FATAL_ERROR "OPENCV_DIR environment variable is not set")
endif ()
<% } %>

set(CMAKE_PREFIX_PATH ${CMAKE_PREFIX_PATH} $ENV{QT5_PREFIX})
<% if (opencv) { %>
set(OpenCV_DIR $ENV{OPENCV_DIR})
<% } %>

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

set(CMAKE_INSTALL_PREFIX ${CMAKE_CURRENT_BINARY_DIR}/install)

find_package(Qt5 COMPONENTS Core Widgets Concurrent REQUIRED)
<% if (opencv) { %>
find_package(OpenCV REQUIRED core imgcodecs videoio)
<% } %>

set(COMPILE_OPTIONS -std=c++17)


add_executable(<%= project %> WIN32
        src/main.cpp
        src/main_window.h
        src/main_window.cpp)

macro(copy_qt _target)
    add_custom_target(copy_dll_${_target}
            COMMAND ${CMAKE_COMMAND} -E copy_if_different "$ENV{QT5_PREFIX}/bin/Qt5Core$<$<CONFIG:Debug>:d>.dll" "${CMAKE_CURRENT_BINARY_DIR}"
            COMMAND ${CMAKE_COMMAND} -E copy_if_different "$ENV{QT5_PREFIX}/bin/Qt5Concurrent$<$<CONFIG:Debug>:d>.dll" "${CMAKE_CURRENT_BINARY_DIR}"
            COMMAND ${CMAKE_COMMAND} -E copy_if_different "$ENV{QT5_PREFIX}/bin/Qt5Widgets$<$<CONFIG:Debug>:d>.dll" "${CMAKE_CURRENT_BINARY_DIR}"
            COMMAND ${CMAKE_COMMAND} -E copy_if_different "$ENV{QT5_PREFIX}/bin/Qt5Gui$<$<CONFIG:Debug>:d>.dll" "${CMAKE_CURRENT_BINARY_DIR}"
            COMMENT "Copy 3rd party dlls dependencies"
            )
    add_dependencies(${_target} copy_dll_${_target})
endmacro()

<% if (opencv) { %>
macro(copy_opencv_dll _target)
    add_custom_target(copy_opencv_dll_${_target}
            COMMAND ${CMAKE_COMMAND} -E copy_if_different "$ENV{OPENCV_DIR}/${OpenCV_ARCH}/${OpenCV_RUNTIME}/bin/opencv_world440$<$<CONFIG:Debug>:d>.dll"
            "${CMAKE_CURRENT_BINARY_DIR}")
    add_dependencies(${_target} copy_opencv_dll_${_target})
endmacro()
<% } %>

target_link_libraries(<%= project %>
        Qt5::Core
        Qt5::Widgets
        Qt5::Concurrent
        <% if (opencv) { %>
        opencv_core
        opencv_videoio
        <% } %>
        )

copy_qt(<%= project %>)
<% if (opencv) { %>
copy_opencv_dll(<%= project %>)
<% } %>

set(INSTALL_PATH ${PROJECT_BINARY_DIR}/install)

string(REPLACE "\\" "/" QT5_INST $ENV{QT5_PREFIX}/bin/Qt5Core$<$<CONFIG:Debug>:d>.dll)
string(REPLACE "\\" "/" QT5_WIDGETS_INST $ENV{QT5_PREFIX}/bin/Qt5Widgets$<$<CONFIG:Debug>:d>.dll)
string(REPLACE "\\" "/" QT5_GUI_INST $ENV{QT5_PREFIX}/bin/Qt5Gui$<$<CONFIG:Debug>:d>.dll)
string(REPLACE "\\" "/" QT5_CONCURRENT_INST $ENV{QT5_PREFIX}/bin/Qt5Concurrent$<$<CONFIG:Debug>:d>.dll)
<% if (opencv) { %>
string(REPLACE "\\" "/" OPENCV_INST $ENV{OPENCV_DIR}/${OpenCV_ARCH}/${OpenCV_RUNTIME}/bin/opencv_world440$<$<CONFIG:Debug>:d>.dll)
<% } %>
string(REPLACE "\\" "/" ENV_QT5_PREFIX $ENV{QT5_PREFIX})

install(TARGETS <%= project %>
        ARCHIVE DESTINATION ${INSTALL_PATH} COMPONENT exec
        RUNTIME DESTINATION ${INSTALL_PATH} COMPONENT exec
        )

install(FILES
        ${QT5_INST}
        ${QT5_WIDGETS_INST}
        ${QT5_GUI_INST}
        ${QT5_CONCURRENT_INST}
        <% if (opencv) { %>
        ${OPENCV_INST}
        <% } %>
        ${PROJECT_SOURCE_DIR}/README.md
        DESTINATION ${INSTALL_PATH} COMPONENT exec)

install(FILES
        ${ENV_QT5_PREFIX}/plugins/platforms/qminimal.dll
        ${ENV_QT5_PREFIX}/plugins/platforms/qoffscreen.dll
        ${ENV_QT5_PREFIX}/plugins/platforms/qdirect2d.dll
        ${ENV_QT5_PREFIX}/plugins/platforms/qwindows.dll
        DESTINATION ${INSTALL_PATH}/plugins/platforms COMPONENT exec)

add_custom_target(install_exec
        COMMAND ${CMAKE_COMMAND} -D COMPONENT=exec -P ${CMAKE_BINARY_DIR}/cmake_install.cmake
        DEPENDS $<TARGET_FILE:<%= project %>>
        )