/**
  @brief os type branch
  @author bodguy
  @date 17.07.17
  @todo
  @bug
*/

#ifndef os_types_h
#define os_types_h

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
#define GLEW_INCLUDE_DIR2 ""
#elif defined(__APPLE__) || defined(__MACH__)
#define Theodore_MacOS
#define GLEW_INCLUDE_DIR <GL/glew.h>
#define GLEW_INCLUDE_DIR2 ""
#endif

#endif /* os_types_h */
