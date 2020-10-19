cmake_minimum_required(VERSION 3.5)

project(Rasterizer LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# QtCreator supports the following variables for Android, which are identical to qmake Android variables.
# Check http://doc.qt.io/qt-5/deployment-android.html for more information.
# They need to be set before the find_package(Qt5 ...) call.

#if(ANDROID)
#    set(ANDROID_PACKAGE_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/android")
#    if (ANDROID_ABI STREQUAL "armeabi-v7a")
#        set(ANDROID_EXTRA_LIBS
#            ${CMAKE_CURRENT_SOURCE_DIR}/path/to/libcrypto.so
#            ${CMAKE_CURRENT_SOURCE_DIR}/path/to/libssl.so)
#    endif()
#endif()

find_package(Qt5 COMPONENTS Widgets REQUIRED)

set(SRC_DIR ${CMAKE_CURRENT_SOURCE_DIR}/src)
set(DEP_DIR ${CMAKE_CURRENT_SOURCE_DIR}/deps)
set(GLM_DIR ${DEP_DIR}/glm-0.9.9.3)

## Add source files
#file(GLOB_RECURSE SOURCE_FILES
#        ${CMAKE_SOURCE_DIR}/src/*.c
#        ${CMAKE_SOURCE_DIR}/src/*.cpp)

## Add header files
#file(GLOB_RECURSE HEADER_FILES
#        ${CMAKE_SOURCE_DIR}/src/*.h
#        ${CMAKE_SOURCE_DIR}/src/*.hpp)

include_directories(${GLM_DIR}/glm)
#link_directories(${GLM_DIR}/glm)
#add_definitions(${GLM_DIR}/glm)

if(ANDROID)
  add_library(Rasterizer SHARED
    main.cpp
    mainwindow.cpp
    mainwindow.h
    mainwindow.ui
  )
else()
  add_executable(Rasterizer
    ${SRC_DIR}/main.cpp
    ${SRC_DIR}/mainwindow.cpp
    ${SRC_DIR}/mainwindow.h
    ${SRC_DIR}/mainwindow.ui
    ${SRC_DIR}/ray.h
  )
endif()

target_link_libraries(Rasterizer PRIVATE Qt5::Widgets)
