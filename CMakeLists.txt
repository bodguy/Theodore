cmake_minimum_required(VERSION 3.6)

# Fix behavior of CMAKE_CXX_STANDARD when targeting macOS.
if(POLICY CMP0025)
    cmake_policy(SET CMP0025 NEW)
endif()

project(Theodore LANGUAGES CXX VERSION 0.1.0)
message(STATUS "CMake version: ${CMAKE_VERSION}")

# for old cmake version compatible
macro(use_cxx11)
    if(CMAKE_VERSION VERSION_LESS "3.1")
        if(CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
            set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=gnu++11")
        endif()
    else()
        set (CMAKE_CXX_STANDARD 11)
    endif()
endmacro(use_cxx11)

use_cxx11()
set(CMAKE_WARN_DEPRECATED 0)
set(GLEW_INCLUDE_DIRS "third_party/include/")
set(SOURCE_PREFIX "src")

set(THEODORE_INCLUDE_DIR "${CMAKE_SOURCE_DIR}/${SOURCE_PREFIX}")
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/")

# MacOS lib settings
if(APPLE)
    message(STATUS "Platform detected: Macos")
    find_library(COCOA_LIBRARY Cocoa REQUIRED)
    find_library(OPENGL_LIBRARY OpenGL REQUIRED)
    set(EXTRA_LIBS ${COCOA_LIBRARY} ${OPENGL_LIBRARY})
    set(GLEW_LIBRARIES_DIRS "third_party/lib/Mac/")
    link_libraries(libGLEW.a)
endif(APPLE)

# Linux lib settings
if(LINUX)
    message(STATUS "Platform detected: Linux")
#    include_directories("/usr/X11R6/include/")
#    link_directories("/usr/X11R6/lib")
    find_library(X11_LIBRARY X11 REQUIRED)
    find_library(OPENGL_LIBRARY GL REQUIRED)
    set(EXTRA_LIBS ${OPENGL_LIBRARY} ${X11_LIBRARY})
    set(GLEW_LIBRARIES_DIRS "third_party/lib/Linux/")
    link_libraries(libGLEW.a)
endif(LINUX)

# Windows lib settings
if(WIN32)
    message(STATUS "Platform detected: Windows")
    find_package(OpenGL REQUIRED)
    find_package(WindowsSDK REQUIRED)
    set(EXTRA_LIBS opengl32)
    # detect cpu architecture
    math(EXPR BITS "8*${CMAKE_SIZEOF_VOID_P}")
    message(STATUS "Detected cpu archetecture: ${BITS}bits")
    set(GLEW_LIBRARIES_DIRS "third_party/lib/Win${BITS}/")
    link_libraries(glew32.lib)
endif(WIN32)

message(STATUS "Library linking: GLEW linking")
include_directories(${GLEW_INCLUDE_DIRS})
link_directories(${GLEW_LIBRARIES_DIRS})

# release build path
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/debug)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE "${CMAKE_SOURCE_DIR}/release")

# resource path
configure_file("${CMAKE_SOURCE_DIR}/cmake/RootDir.h.in" "${THEODORE_INCLUDE_DIR}/RootDir.h")

file(GLOB_RECURSE SOURCE_FILES ${SOURCE_PREFIX}/*.h ${SOURCE_PREFIX}/*.cpp ${SOURCE_PREFIX}/*.hpp ${SOURCE_PREFIX}/*.mm)
add_library(${CMAKE_PROJECT_NAME} STATIC ${SOURCE_FILES})
target_link_libraries(${CMAKE_PROJECT_NAME} ${EXTRA_LIBS})
target_include_directories(${CMAKE_PROJECT_NAME} PUBLIC ${THEODORE_INCLUDE_DIR})

# Create virtual folders to make it look nicer in VS
if(MSVC_IDE)
    # Macro to preserve source files hierarchy in the IDE
    macro(GroupSources curdir)
        file(GLOB children RELATIVE ${PROJECT_SOURCE_DIR}/${curdir} ${PROJECT_SOURCE_DIR}/${curdir}/*)

        foreach(child ${children})
            if(IS_DIRECTORY ${PROJECT_SOURCE_DIR}/${curdir}/${child})
                GroupSources(${curdir}/${child})
            else()
                string(REPLACE "/" "\\" groupname ${curdir})
                string(REPLACE "src" "Sources" groupname ${groupname})
                source_group(${groupname} FILES ${PROJECT_SOURCE_DIR}/${curdir}/${child})
            endif()
        endforeach()
    endmacro()

    # Run macro
    GroupSources(src)
    # disable warning
    add_definitions(/wd4267) #remove size_t to int warning
    add_definitions(/wd4244) #remove double to float conversion warning
    add_definitions(/wd4305) #remove double to float truncation warning
endif()

# make sample
function(add_sample TARGET ENTRYPOINT)
    source_group("" FILES ${ENTRYPOINT})
    add_executable(${TARGET} ${ENTRYPOINT})
    target_link_libraries(${TARGET} ${CMAKE_PROJECT_NAME})
endfunction()

set(PROJECT_SAMPLE_DIR "${CMAKE_SOURCE_DIR}/sample")
add_sample(01-hello_world "${PROJECT_SAMPLE_DIR}/01-hello_world/main.cpp")
add_sample(02-triangle "${PROJECT_SAMPLE_DIR}/02-triangle/main.cpp")

# unit test target
find_package(Catch2 REQUIRED)
set(PROJECT_UNIT_TEST_NAME "${CMAKE_PROJECT_NAME}_unit_test")
set(PROJECT_TEST_DIR "${CMAKE_SOURCE_DIR}/test")
file(GLOB_RECURSE TEST_SOURCE_FILES "${PROJECT_TEST_DIR}/*.cpp")

add_executable(${PROJECT_UNIT_TEST_NAME} ${TEST_SOURCE_FILES})
target_link_libraries(${PROJECT_UNIT_TEST_NAME} Catch2::Catch2 ${CMAKE_PROJECT_NAME})

include(CTest)
include(Catch)
catch_discover_tests(${PROJECT_UNIT_TEST_NAME})