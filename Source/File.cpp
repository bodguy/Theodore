#include "File.h"
#include <stdarg.h>
#pragma warning(disable:4996)

namespace Quark {
    File::File() : fp(NULL), mFileName(nullptr), mMode(Enumeration::Read) {
    }
    
    File::File(const std::string& name, Enumeration::OpenMode accessType) {
        Open(name, accessType);
    }
    
    File::File(FILE *file): fp(file) {
    }
    
    File::~File() {
        Clear();
    }
    
    bool File::IsOpen() {
        if (!fp)
            return false;
        
        return true;
    }
    
    bool File::Open(const std::string& name, Enumeration::OpenMode access_type) {
        mFileName = name.c_str();
        mMode = access_type;
        
        switch (access_type) {
            case Enumeration::Read:
                fp = fopen(mFileName, "r");
                //assert(fp && "file is not exist!");
                break;
            case Enumeration::Write:
                fp = fopen(mFileName, "w");
                //assert(fp && "file is not exist!");
                break;
            case Enumeration::ReadWrite:
                fp = fopen(mFileName, "w+");
                //assert(fp && "file is not exist!");
                break;
            case Enumeration::Append:
                fp = fopen(mFileName, "a");
                //assert(fp && "file is not exist!");
                break;
            case Enumeration::ReadBinary:
                fp = fopen(mFileName, "rb");
                //assert(fp && "file is not exist!");
                break;
            case Enumeration::WriteBinary:
                fp = fopen(mFileName, "wb");
                //assert(fp && "file is not exist!");
                break;
            case Enumeration::ReadWriteBinary:
                fp = fopen(mFileName, "wb+");
                //assert(fp && "file is not exist!");
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
    
    void File::Write(const char *format, ...) {
        va_list args;
        va_start(args, format);
        vfprintf(fp, format, args);
        va_end(args);
    }
    
    void File::Read(const char *format, ...) {
        //va_list args;
        //va_start(args, format);
        //vfscanf(fp, format, args);
        //va_end(args);
    }
    
    std::string File::ReadUntilEnd() {
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
    
    std::string File::GetLine(const char *delims) {
        std::string temp;
        
        char c;
        
        for (;;) {
            c = static_cast<char>(fgetc(fp));
            if (feof(fp))
                break;
            for (const char *temp_delim = delims; temp_delim; ++temp_delim)
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
    
    void File::Rewind() {
        SeekFromBeginOfFile(0);
    }
    
    void File::SeekFromBeginOfFile(int offset) {
        fseek(fp, offset, SEEK_SET);
    }
    
    void File::SeekFromEndOfFile(int offset) {
        fseek(fp, offset, SEEK_END);
    }
    
    void File::SeekByOffset(int offset) {
        fseek(fp, offset, SEEK_CUR);
    }
    
    bool File::Validate(void) const {
        return fp && !feof(fp) ? true : false;
    }
}
