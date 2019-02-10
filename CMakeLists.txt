cmake_minimum_required(VERSION 3.6)
project(Theodore LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_WARN_DEPRECATED 0)
set(GLEW_INCLUDE_DIRS "libs/include/")
set(SOURCE_PREFIX "src/")

# MacOS lib settings
if(APPLE)
    message(STATUS ">>>[Platform detected]: Macos")
    include_directories("/System/Library/Frameworks")
    find_library(COCOA_LIBRARY Cocoa)
    find_library(OpenGL_LIBRARY OpenGL)
    mark_as_advanced(COCOA_LIBRARY OpenGL_LIBRARY)
    set(EXTRA_LIBS ${COCOA_LIBRARY} ${OpenGL_LIBRARY})
    set(GLEW_LIBRARIES "libs/lib/Mac/")
    link_libraries(libGLEW.a)
endif(APPLE)

# Linux lib settings
if(LINUX)
    message(STATUS ">>>[Platform detected]: Linux")
    include_directories("/usr/X11R6/include/")
    link_directories("/usr/X11R6/lib")
    set(EXTRA_LIBS GL X11 GLU)
    set(GLEW_LIBRARIES "libs/lib/Linux/")
    link_libraries(libGLEW.a)
endif(LINUX)

# Windows lib settings
if(WIN32)
    message(STATUS ">>>[Platform detected]: Windows")
    set(GLEW_LIBRARIES "libs/lib/Win32/")
    link_libraries(glew32.lib)
endif(WIN32)

message(STATUS ">>>[Library linking]: GLEW linking")
include_directories(${GLEW_INCLUDE_DIRS})
link_directories(${GLEW_LIBRARIES})

file(GLOB_RECURSE SOURCE_FILES ${SOURCE_PREFIX}*.cpp ${SOURCE_PREFIX}*.h ${SOURCE_PREFIX}*.mm)
add_executable(${CMAKE_PROJECT_NAME} ${SOURCE_FILES})
target_link_libraries(${CMAKE_PROJECT_NAME} ${EXTRA_LIBS})