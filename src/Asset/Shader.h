/**
  @brief OpenGL shader compile and managing
  @author bodguy
  @date 17.07.17
  @todo
  @bug
*/

#ifndef Shader_h
#define Shader_h

#include "../Graphics/Enumeration.h"
#include "../Platform/os_types.h"
#include GLEW_INCLUDE_DIR
#include <map>
#include <string>
#include "Asset.h"

namespace Theodore {
  typedef int Attribute;
  typedef int Uniform;

  struct IncludeInfo {
    int offset;
    int end;
    char *filename;
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

    static char* PreprocessIncludes(char* str, char* path, char error[256]);
    static void FreeIncludes(IncludeInfo* array, int len);
    static char* LoadFile(char* filename, size_t* plen);
    static char* IncludeFile(char* filename, char* path, char error[256]);
    static int FindInclude(char* text, IncludeInfo** plist);
    static IncludeInfo* AppendInclude(IncludeInfo* array, int len, int offset, int end, char* filename, int next_line);
    static char* Append(char* str, size_t* curlen, char* addstr, size_t addlen);
    static int IsSpace(int ch);

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
