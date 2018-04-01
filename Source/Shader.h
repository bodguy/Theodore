#ifndef Shader_h
#define Shader_h

#include "Enumeration.h"
#include "os_types.h"
#include GLEW_INCLUDE_DIR
#include <string>

namespace Quark {
    typedef int Attribute;
    typedef int Uniform;
    
    class Shader {
        friend class Program;
    public:
        Shader(const Enumeration::ShaderType type);
        ~Shader();
        
        int Compile(const std::string& source);
        int IsCompiled() const;
        static std::string PreprocessIncludes(const std::string& source, int level = 0);
        
    private:
        unsigned int mShaderID;
        int mIsCompiled;
    };
    
    class Vector4d; class Vector3d; class Vector2d; class Matrix4x4; class Color;
    class Program {
    public:
        Program();
        Program(const Shader& vertex);
        Program(const Shader& vertex, const Shader& fragment);
        Program(const Shader& vertex, const Shader& fragment, const Shader& geometry);
		Program(const Shader& vertex, const Shader& fragment, const Shader& geometry, const Shader& tessControl, const Shader& tessEval);
        ~Program();
        
        void AttachShader(const Shader& shader);
        void DetachShader(const Shader& shader);
        int Link();
        void Use();
        unsigned int GetProgramID() const;
        
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
        void SetUniformBlock(const Uniform& uniform, const unsigned int bindingPoint);
        
    private:
        unsigned int mProgramID;
    };
}

#endif /* Shader_h */
