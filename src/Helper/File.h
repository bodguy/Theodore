/**
  @brief file IO utility
  @author bodguy
  @date 17.07.17
  @todo
  @bug
*/

#ifndef File_h
#define File_h

#include <string>
#include "../Graphics/Enumeration.h"

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
    void Write(const char* format, ...);
    void Read(const char* format, ...);
    std::string ReadFile();

    template <typename T, size_t size>
    size_t WriteBinary(const T (&buffer)[size]);
    size_t WriteBinary(void* buffer, size_t elementsize, size_t elementcount);
    template <typename T, size_t size>
    size_t ReadBinary(T (&buffer)[size]);
    size_t ReadBinary(void* buffer, size_t elementsize, size_t elementcount);

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
    std::string BaseName(const std::string& path);
    std::string RemoveExtension(const std::string& filename);

#if (_MSC_VER == 1700)
    int vfscanf(FILE* file, const char* format, va_list argPtr);
#endif
    FILE* fp;  // Pointer to file
    std::string mBaseName;
    std::string mFileName;
    std::string mFullFileName;
    OpenMode mMode;
  };

  template <typename T, size_t size>
  size_t File::WriteBinary(const T (&buffer)[size]) {
    if (IsOpen()) {
      return fwrite(&buffer, sizeof(T), size, fp);
    }

    return 0;
  }

  template <typename T, size_t size>
  size_t File::ReadBinary(T (&buffer)[size]) {
    if (IsOpen()) {
      return fread(&buffer, sizeof(T), size, fp);
    }

    return 0;
  }
}  // namespace Theodore

#endif /* File_h */
