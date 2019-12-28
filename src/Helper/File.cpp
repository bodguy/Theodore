// Copyright (C) 2017 by bodguy
// This code is licensed under Apache 2.0 license (see LICENSE.md for details)

#include "File.h"

#include <stdarg.h>
#pragma warning(disable : 4996)

namespace Theodore {
  File::File() : fp(NULL), mBaseName(""), mFileName(""), mFullFileName(""), mMode(OpenMode::Read) {}

  File::File(const std::string& name, OpenMode accessType) { Open(name, accessType); }

  File::File(FILE* file) : fp(file), mBaseName(""), mFileName(""), mFullFileName(""), mMode(OpenMode::Read) {}

  File::~File() { Clear(); }

  bool File::IsOpen() {
    if (!fp) return false;

    return true;
  }

  bool File::Open(const std::string& name, OpenMode access_type) {
    mFullFileName = std::string(name);
    mBaseName = BaseName(mFullFileName);
    mFileName = RemoveExtension(mBaseName);
    mMode = access_type;

    switch (access_type) {
      case OpenMode::Read:
        fp = fopen(mFullFileName.c_str(), "r");
        break;
      case OpenMode::Write:
        fp = fopen(mFullFileName.c_str(), "w");
        break;
      case OpenMode::ReadWrite:
        fp = fopen(mFullFileName.c_str(), "w+");
        break;
      case OpenMode::Append:
        fp = fopen(mFullFileName.c_str(), "a");
        break;
      case OpenMode::ReadBinary:
        fp = fopen(mFullFileName.c_str(), "rb");
        break;
      case OpenMode::WriteBinary:
        fp = fopen(mFullFileName.c_str(), "wb");
        break;
      case OpenMode::ReadWriteBinary:
        fp = fopen(mFullFileName.c_str(), "wb+");
        break;
      default:
        return false;
    }

    return IsOpen();
  }

  void File::Close(void) {
    if (IsOpen()) {
      fclose(fp);
      fp = NULL;
    }
  }

  void File::Clear(void) {
    Close();
    mBaseName = "";
    mFileName = "";
    mFullFileName = "";
    mMode = OpenMode::Read;
  }

  void File::Writef(const char* format, ...) {
    va_list args;
    va_start(args, format);
    vfprintf(fp, format, args);
    va_end(args);
  }

  void File::Readf(const char* format, ...) {
    va_list args;
    va_start(args, format);
    vfscanf(fp, format, args);
    va_end(args);
  }

  std::string File::ReadFile() {
    std::string str;

    if (IsOpen()) {
      std::string Line = "";
      while (Validate()) {
        Line = GetLine();
        str += "\n" + Line;
      }
    }

    return str;
  }

  size_t File::WriteBuf(void* buffer, size_t elementsize, size_t elementcount) {
    if (IsOpen()) {
      return fwrite(buffer, elementsize, elementcount, fp);
    }

    return 0;
  }

  size_t File::ReadBuf(void* buffer, size_t elementsize, size_t elementcount) {
    if (IsOpen()) {
      return fread(buffer, elementsize, elementcount, fp);
    }

    return 0;
  }

  std::string File::GetLine(void) {
    std::string temp;

    char c;

    for (;;) {
      c = static_cast<char>(fgetc(fp));
      if (feof(fp))
        break;
      else if (c == '\n')
        break;
      temp += c;
    }

    return temp;
  }

  std::string File::GetLine(char delim) {
    std::string temp;

    char c;

    for (;;) {
      c = static_cast<char>(fgetc(fp));
      if (feof(fp))
        break;
      else if (c == delim)
        break;
      temp += c;
    }

    return temp;
  }

  std::string File::GetLine(const char* delims) {
    std::string temp;

    char c;

    for (;;) {
      c = static_cast<char>(fgetc(fp));
      if (feof(fp)) break;
      for (const char* temp_delim = delims; temp_delim; ++temp_delim)
        if (c == *temp_delim) break;
      temp += c;
    }

    return temp;
  }

  size_t File::GetSize() {
    SeekFromEndOfFile(0);
    size_t size = ftell(fp);
    Rewind();
    return size;
  }

  void File::Rewind() { SeekFromBeginOfFile(0); }

  void File::SeekFromBeginOfFile(int offset) { fseek(fp, offset, SEEK_SET); }

  void File::SeekFromEndOfFile(int offset) { fseek(fp, offset, SEEK_END); }

  void File::SeekByOffset(int offset) { fseek(fp, offset, SEEK_CUR); }

  bool File::Validate(void) const { return fp && !feof(fp) ? true : false; }

  std::string File::GetFullName() const { return mFullFileName; }

  std::string File::GetBaseName() const { return mBaseName; }

  std::string File::GetFileName() const { return mFileName; }

  std::string File::BaseName(const std::string& path) {
    const size_t last_slash_idx = path.find_last_of("\\/");
    if (std::string::npos != last_slash_idx) {
      return path.substr(last_slash_idx + 1);
    }

    return std::string("");
  }

  std::string File::RemoveExtension(const std::string& filename) {
    const size_t period_idx = filename.find_last_of('.');
    if (std::string::npos != period_idx) {
      return filename.substr(0, period_idx);
    }

    return std::string("");
  }

#if (_MSC_VER == 1700) && defined(ENVIRONMENT32)  // _asm only works in x86
  // In visual studio 2012, vfscanf are not supported so, we implement it from bottom.
  int File::vfscanf(FILE* file, const char* format, va_list argPtr) {
    // http://www.codeguru.com/Cpp/Cpp/string/comments.php/c5631/?thread=61724

    // Get an upper bound for the # of args
    size_t count = 0;
    const char* p = format;

    while (1) {
      char c = *(p++);
      if (c == 0) break;

      if (c == '%' && (p[0] != '*' && p[0] != '%')) ++count;
    }

    if (count <= 0) return 0;

    int result;

    // copy stack pointer
    _asm
    {
			mov esi, esp;
    }

    // push variable parameters pointers on stack
    for (int i = count - 1; i >= 0; --i) {
      _asm
      {
				mov eax, dword ptr[i];
				mov ecx, dword ptr[argPtr];
				mov edx, dword ptr[ecx + eax * 4];
				push edx;
      }
    }

    int stackAdvance = (2 + count) * 4;

    _asm
    {
      // now push on the fixed params
			mov eax, dword ptr[format];
			push eax;
			mov eax, dword ptr[file];
			push eax;

      // call fscanf, and more the result in to result
			call dword ptr[fscanf];
			mov result, eax;

      // restore stack pointer
			mov eax, dword ptr[stackAdvance];
			add esp, eax;
      // mov esp, esi;
    }

    return result;
  }

#elif (_MSC_VER == 1700) && defined(ENVIRONMENT64)
  int File::vfscanf(FILE* file, const char* format, va_list argPtr) {
    __debugbreak();
    return 0;
  }
#endif
}  // namespace Theodore
