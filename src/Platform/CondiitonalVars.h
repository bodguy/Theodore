// Copyright (C) 2017 by bodguy
// This code is licensed under Apache 2.0 license (see LICENSE.md for details)

#ifndef os_types_h
#define os_types_h

#define GLEW_NO_GLU

// platform detection
#if defined(_WIN32) || defined(_WIN64)
#if _WIN64
#define ENVIRONMENT64
#else
#define ENVIRONMENT32
#endif
#define Theodore_Windows
#define GLEW_INCLUDE_DIR <GL/glew.h>
#define GLEW_INCLUDE_DIR2 <GL/wglew.h>
#elif defined(__linux__)
#define Theodore_Linux
#define GLEW_INCLUDE_DIR <GL/glx.h>
#elif defined(__APPLE__) || defined(__MACH__)
#define Theodore_MacOS
#define GLEW_INCLUDE_DIR <GL/glew.h>
#endif

#endif /* os_types_h */
