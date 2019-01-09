#include "File.h"
#include <stdarg.h>
#pragma warning(disable : 4996)

namespace Theodore {
  File::File() : fp(NULL), mFileName(nullptr), mMode(OpenMode::Read) {}

  File::File(const std::string& name, OpenMode accessType) { Open(name, accessType); }

  File::File(FILE* file) : fp(file) {}

  File::~File() { Clear(); }

  bool File::IsOpen() {
    if (!fp)
      return false;

    return true;
  }

  bool File::Open(const std::string& name, OpenMode access_type) {
    mFileName = name.c_str();
    mMode = access_type;

    switch (access_type) {
    case OpenMode::Read:
      fp = fopen(mFileName, "r");
      // assert(fp && "file is not exist!");
      break;
    case OpenMode::Write:
      fp = fopen(mFileName, "w");
      // assert(fp && "file is not exist!");
      break;
    case OpenMode::ReadWrite:
      fp = fopen(mFileName, "w+");
      // assert(fp && "file is not exist!");
      break;
    case OpenMode::Append:
      fp = fopen(mFileName, "a");
      // assert(fp && "file is not exist!");
      break;
    case OpenMode::ReadBinary:
      fp = fopen(mFileName, "rb");
      // assert(fp && "file is not exist!");
      break;
    case OpenMode::WriteBinary:
      fp = fopen(mFileName, "wb");
      // assert(fp && "file is not exist!");
      break;
    case OpenMode::ReadWriteBinary:
      fp = fopen(mFileName, "wb+");
      // assert(fp && "file is not exist!");
      break;
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
    mFileName = NULL;
  }

  void File::Write(const char* format, ...) {
    va_list args;
    va_start(args, format);
    vfprintf(fp, format, args);
    va_end(args);
  }

  void File::Read(const char* format, ...) {
    va_list args;
    va_start(args, format);
    vfscanf(fp, format, args);
    va_end(args);
  }

  std::string File::ReadAllText() {
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

  size_t File::WriteBinary(void* buffer, size_t elementsize, size_t elementcount) {
    if (IsOpen()) {
      return fwrite(buffer, elementsize, elementcount, fp);
    }

    return 0;
  }

  size_t File::ReadBinary(void* buffer, size_t elementsize, size_t elementcount) {
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
      if (feof(fp))
        break;
      for (const char* temp_delim = delims; temp_delim; ++temp_delim)
        if (c == *temp_delim)
          break;
      temp += c;
    }

    return temp;
  }

  size_t File::GetSize() {
    SeekFromEndOfFile(0);
    size_t size = ftell(fp);
    SeekFromBeginOfFile(0);
    return size;
  }

  void File::Rewind() { SeekFromBeginOfFile(0); }

  void File::SeekFromBeginOfFile(int offset) { fseek(fp, offset, SEEK_SET); }

  void File::SeekFromEndOfFile(int offset) { fseek(fp, offset, SEEK_END); }

  void File::SeekByOffset(int offset) { fseek(fp, offset, SEEK_CUR); }

  bool File::Validate(void) const { return fp && !feof(fp) ? true : false; }

#if (_MSC_VER == 1700) && defined(ENVIRONMENT32) // _asm only works in x86
  // In visual studio 2012, vfscanf are not supported so, we implement it from bottom.
  int File::vfscanf(FILE* file, const char* format, va_list argPtr) {
    // http://www.codeguru.com/Cpp/Cpp/string/comments.php/c5631/?thread=61724

    // Get an upper bound for the # of args
    size_t count = 0;
    const char* p = format;

    while (1) {
      char c = *(p++);
      if (c == 0)
        break;

      if (c == '%' && (p[0] != '*' && p[0] != '%'))
        ++count;
    }

    if (count <= 0)
      return 0;

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
}
