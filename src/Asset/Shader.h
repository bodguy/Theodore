// Copyright (C) 2017 by bodguy
// This code is licensed under Apache 2.0 license (see LICENSE.md for details)

#ifndef Shader_h
#define Shader_h

#include <string>
#include <vector>
#include "Asset.h"
#include "Graphics/Enumeration.h"

namespace Theodore {
  typedef int Attribute;
  typedef int Uniform;

  struct ShaderPreprocess {
    ShaderPreprocess(int offset, int end, char* filename, int next_line_after)
    	: offset(offset), end(end), filename(filename), next_line_after(next_line_after) {}

    int offset;
    int end;
    char* filename;
    int next_line_after;
  };

  class Pipeline;
  class Shader : public Asset {
    friend class Pipeline;

  public:
    explicit Shader(ShaderType type);
    virtual ~Shader() override;

    int Compile(const std::string& source);
    static Pipeline* Find(const std::string& name);

  private:
    static char* PreprocessFromString(char* str, const std::string& path);
    static char* PreprocessFromFile(const std::string& filename, const std::string& path);
    static int RetrieveDirective(char* text, std::vector<ShaderPreprocess*>& plist);
    static char* Read(const std::string& filename);
    static void Free(const std::vector<ShaderPreprocess*>& list);
    static char* ReAlloc(char* str, size_t* curlen, char* addstr, size_t addlen);

  private:
    unsigned int shaderId;
  };
}  // namespace Theodore

#endif /* Shader_h */
