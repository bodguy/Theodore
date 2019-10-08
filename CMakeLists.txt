cmake_minimum_required(VERSION 3.6)

# Fix behavior of CMAKE_CXX_STANDARD when targeting macOS.
if (POLICY CMP0025)
    cmake_policy(SET CMP0025 NEW)
endif ()

project(Theodore LANGUAGES CXX VERSION 0.1.0)

# for old cmake version compatible
macro(use_cxx11)
    if (CMAKE_VERSION VERSION_LESS "3.1")
        if (CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
            set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=gnu++11")
        endif ()
    else ()
        set (CMAKE_CXX_STANDARD 11)
    endif ()
endmacro(use_cxx11)

use_cxx11()
set(CMAKE_WARN_DEPRECATED 0)
set(GLEW_INCLUDE_DIRS "third_party/include/")
set(SOURCE_PREFIX "src/")

set(THEODORE_INCLUDE_DIR "${CMAKE_SOURCE_DIR}/${SOURCE_PREFIX}")

# MacOS lib settings
if(APPLE)
    message(STATUS ">>>[Platform detected]: Macos")
    include_directories("/System/Library/Frameworks")
    find_library(COCOA_LIBRARY Cocoa)
    find_library(OpenGL_LIBRARY OpenGL)
    mark_as_advanced(COCOA_LIBRARY OpenGL_LIBRARY)
    set(EXTRA_LIBS ${COCOA_LIBRARY} ${OpenGL_LIBRARY})
    set(GLEW_LIBRARIES "third_party/lib/Mac/")
    link_libraries(libGLEW.a)
endif(APPLE)

# Linux lib settings
if(LINUX)
    message(STATUS ">>>[Platform detected]: Linux")
    include_directories("/usr/X11R6/include/")
    link_directories("/usr/X11R6/lib")
    set(EXTRA_LIBS GL X11 GLU)
    set(GLEW_LIBRARIES "third_party/lib/Linux/")
    link_libraries(libGLEW.a)
endif(LINUX)

# Windows lib settings
if(WIN32)
    message(STATUS ">>>[Platform detected]: Windows")
    set(GLEW_LIBRARIES "third_party/lib/Win32/")
    link_libraries(glew32.lib)
endif(WIN32)

message(STATUS ">>>[Library linking]: GLEW linking")
include_directories(${GLEW_INCLUDE_DIRS})
link_directories(${GLEW_LIBRARIES})

# release build path
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE "${CMAKE_SOURCE_DIR}/release")

file(GLOB_RECURSE SOURCE_FILES ${SOURCE_PREFIX}*.cpp ${SOURCE_PREFIX}*.hpp ${SOURCE_PREFIX}*.mm)
add_executable(${CMAKE_PROJECT_NAME} ${SOURCE_FILES})
target_link_libraries(${CMAKE_PROJECT_NAME} ${EXTRA_LIBS})
target_include_directories(${CMAKE_PROJECT_NAME} PUBLIC ${THEODORE_INCLUDE_DIR})

# unit test
find_package(Catch2 REQUIRED)
set(PROJECT_UNIT_TEST_NAME "${CMAKE_PROJECT_NAME}_unit_test")
set(TEST_PREFIX "test/")
file(GLOB_RECURSE TEST_SOURCE_FILES ${TEST_PREFIX}*.cpp)
add_executable(${PROJECT_UNIT_TEST_NAME} ${TEST_SOURCE_FILES})
target_link_libraries(${PROJECT_UNIT_TEST_NAME} Catch2::Catch2)
include(CTest)
include(Catch)
catch_discover_tests(${PROJECT_UNIT_TEST_NAME})