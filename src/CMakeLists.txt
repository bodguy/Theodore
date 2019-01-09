cmake_minimum_required(VERSION 3.6)
project(CrossPlatform LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 11)
set(GLEW_INCLUDE_DIRS "Libraries/include/GL/")
set(SOURCE_PREFIX "Source/")

# MacOS lib settings
if(APPLE)
    message(STATUS ">>>[Platform detected]: Macos")
    include_directories("/System/Library/Frameworks")
    find_library(COCOA_LIBRARY Cocoa)
    find_library(OpenGL_LIBRARY OpenGL)
    mark_as_advanced(COCOA_LIBRARY OpenGL_LIBRARY)
    set(EXTRA_LIBS ${COCOA_LIBRARY} ${OpenGL_LIBRARY})
    set(GLEW_LIBRARIES "Libraries/lib/Mac/")
endif(APPLE)

# Linux lib settings
if(LINUX)
    message(STATUS ">>>[Platform detected]: Linux")
    include_directories("/usr/X11R6/include/")
    link_directories("/usr/X11R6/lib")
    set(EXTRA_LIBS GL X11 GLU)
    set(GLEW_LIBRARIES "Libraries/lib/Linux/")
endif(LINUX)

# Windows lib settings
if(WIN32)
    message(STATUS ">>>[Platform detected]: Windows")
    set(GLEW_LIBRARIES "Libraries/lib/Win32/")
endif(WIN32)

message(STATUS ">>>[Library linking]: GLEW linking")
include_directories(${GLEW_INCLUDE_DIRS})
link_directories(${GLEW_LIBRARIES})
link_libraries(glew32.lib) # only for windows

file(GLOB SOURCE_FILES ${SOURCE_PREFIX}*.cpp ${SOURCE_PREFIX}*.h ${SOURCE_PREFIX}*.mm)
add_executable(${CMAKE_PROJECT_NAME} ${SOURCE_FILES})
target_link_libraries(${CMAKE_PROJECT_NAME} ${EXTRA_LIBS})
