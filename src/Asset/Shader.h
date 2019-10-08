// Copyright (C) 2017 by bodguy
// This code is licensed under Apache 2.0 license (see LICENSE.md for details)

#ifndef Shader_h
#define Shader_h

#include "Graphics/Enumeration.h"
#include "Platform/os_types.h"
#include "Helper/File.h"
#include "Asset.h"
#include GLEW_INCLUDE_DIR
#include <map>
#include <string>
#include <vector>

namespace Theodore {
  typedef int Attribute;
  typedef int Uniform;

  struct ShaderPreprocess {
    ShaderPreprocess(int offset, int end, char* filename, int next_line_after) : offset(offset), end(end), filename(filename), next_line_after(next_line_after) {}
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
    virtual ~Shader();

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
    unsigned int mShaderID;
  };

  class Vector4d;
  class Vector3d;
  class Vector2d;
  class Matrix4x4;
  class Color;
  class Pipeline {
    friend class ShaderManager;

  public:
    Pipeline(const std::string& name);
    Pipeline(const std::string& name, const Shader& vertex);
    Pipeline(const std::string& name, const Shader& vertex, const Shader& fragment);
    Pipeline(const std::string& name, const Shader& vertex, const Shader& fragment, const Shader& geometry);
    Pipeline(const std::string& name, const Shader& vertex, const Shader& fragment, const Shader& geometry, const Shader& tessControl, const Shader& tessEval);
    ~Pipeline();

    void AttachShader(const Shader& shader);
    void DetachShader(const Shader& shader);
    int Link();
    void Use();
    void UnUse();
    unsigned int GetPipelineID() const;

    Attribute GetAttribute(const std::string& name);
    Uniform GetUniform(const std::string& name);
    Uniform GetUniformBlockIndex(const std::string& name);

    void SetUniform(const Uniform& uniform, int value);
    void SetUniform(const Uniform& uniform, unsigned int value);
    void SetUniform(const Uniform& uniform, float value);
    void SetUniform(const Uniform& uniform, const Vector2d& value);
    void SetUniform(const Uniform& uniform, const Vector3d& value);
    void SetUniform(const Uniform& uniform, const Vector4d& value);
    void SetUniform(const Uniform& uniform, const Color& value);
    void SetUniform(const Uniform& uniform, const float* values, int count);
    void SetUniform(const Uniform& uniform, const Vector2d* values, int count);
    void SetUniform(const Uniform& uniform, const Vector3d* values, int count);
    void SetUniform(const Uniform& uniform, const Vector4d* values, int count);
    void SetUniform(const Uniform& uniform, const Color* values, int count);
    void SetUniform(const Uniform& uniform, const Matrix4x4& value);
    void SetUniform(const Uniform& uniform, bool value);
    void SetUniformBlock(const Uniform& uniform, const unsigned int bindingPoint);

    void SetUniform(const std::string& name, int value);
    void SetUniform(const std::string& name, unsigned int value);
    void SetUniform(const std::string& name, float value);
    void SetUniform(const std::string& name, const Vector2d& value);
    void SetUniform(const std::string& name, const Vector3d& value);
    void SetUniform(const std::string& name, const Vector4d& value);
    void SetUniform(const std::string& name, const Color& value);
    void SetUniform(const std::string& name, const float* values, int count);
    void SetUniform(const std::string& name, const Vector2d* values, int count);
    void SetUniform(const std::string& name, const Vector3d* values, int count);
    void SetUniform(const std::string& name, const Vector4d* values, int count);
    void SetUniform(const std::string& name, const Color* values, int count);
    void SetUniform(const std::string& name, const Matrix4x4& value);
    void SetUniform(const std::string& name, bool value);
    void SetUniformBlock(const std::string& name, const unsigned int bindingPoint);

    void DispatchCompute(unsigned int x, unsigned int y, unsigned int z);

  private:
    unsigned int mPipelineID;
    std::string mName;
  };

  class ShaderManager {
    friend class Shader;

  public:
    ShaderManager();
    ~ShaderManager();

    static bool Append(Pipeline* program);

  private:
    static ShaderManager* shaderManager;
    std::map<std::string, Pipeline*> mPipelines;
  };
}  // namespace Theodore

#endif /* Shader_h */
