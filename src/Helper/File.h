// Copyright (C) 2017 by bodguy
// This code is licensed under Apache 2.0 license (see LICENSE.md for details)

#ifndef File_h
#define File_h

#include <string>
#include "Graphics/Enumeration.h"

namespace Theodore {
  class File {
  public:
    File();
    File(const std::string& name, OpenMode accessType);
    File(FILE* file);
    ~File();

    bool IsOpen();
    bool Open(const std::string& name, OpenMode accessType);
    void Close(void);  // Close file pointer
    void Clear(void);  // Close file pointer (if open) and clear file name
    void Writef(const char* format, ...);
    void Readf(const char* format, ...);
    std::string ReadFile();

    template <typename T, size_t size>
    size_t WriteBuf(const T (&buffer)[size]);
    size_t WriteBuf(void* buffer, size_t elementsize, size_t elementcount);
    template <typename T, size_t size>
    size_t ReadBuf(T (&buffer)[size]);
    size_t ReadBuf(void* buffer, size_t elementsize, size_t elementcount);

    std::string GetLine(void);                // Retrieve one line delimited by '\n'
    std::string GetLine(char delim);          // Retrieve one line delimited by delim
    std::string GetLine(const char* delims);  // Retrieve one line delimited by chars within delims
    size_t GetSize();

    void Rewind();
    void SeekFromBeginOfFile(int offset);
    void SeekFromEndOfFile(int offset);
    void SeekByOffset(int offset);

    bool Validate(void) const;

    std::string GetFullName() const;
    std::string GetBaseName() const;
    std::string GetFileName() const;

  private:
    std::string RemoveExtension(const std::string& filename);

#if (_MSC_VER == 1700)
    int vfscanf(FILE* file, const char* format, va_list argPtr);
#endif
    FILE* filePointer;  // Pointer to file
    std::string baseName;
    std::string fileName;
    std::string fullName;
    OpenMode openMode;
  };

  template <typename T, size_t size>
  size_t File::WriteBuf(const T (&buffer)[size]) {
    if (IsOpen()) {
      return fwrite(&buffer, sizeof(T), size, filePointer);
    }

    return 0;
  }

  template <typename T, size_t size>
  size_t File::ReadBuf(T (&buffer)[size]) {
    if (IsOpen()) {
      return fread(&buffer, sizeof(T), size, filePointer);
    }

    return 0;
  }
}  // namespace Theodore

#endif /* File_h */
