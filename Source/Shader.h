#ifndef Shader_h
#define Shader_h

#include "Enumeration.h"
#include "os_types.h"
#include GLEW_INCLUDE_DIR
#include "Asset.h"
#include <string>
#include <map>

namespace Quark {
    typedef int Attribute;
    typedef int Uniform;
    
    class Shader : public Asset {
        friend class Program;
    public:
        Shader(const ShaderType type);
        virtual ~Shader();
        
        int Compile(const std::string& source);
        int IsCompiled() const;
        static std::string PreprocessIncludes(const std::string& source, int level = 0);
		static Program* Find(const std::string& name);

    private:
        unsigned int mShaderID;
        int mIsCompiled;
    };
    
    class Vector4d; class Vector3d; class Vector2d; class Matrix4x4; class Color;
    class Program {
		friend class ShaderManager;
    public:
        Program(const std::string& name);
        Program(const std::string& name, const Shader& vertex);
        Program(const std::string& name, const Shader& vertex, const Shader& fragment);
        Program(const std::string& name, const Shader& vertex, const Shader& fragment, const Shader& geometry);
		Program(const std::string& name, const Shader& vertex, const Shader& fragment, const Shader& geometry, const Shader& tessControl, const Shader& tessEval);
		~Program();
        
        void AttachShader(const Shader& shader);
        void DetachShader(const Shader& shader);
        int Link();
        void Use();
		void UnUse();
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
        unsigned int mProgramID;
		std::string mName;
    };

	class ShaderManager {
		friend class Shader;
	public:
		ShaderManager();
		~ShaderManager();

		static bool Append(Program* program);

	private:
		static ShaderManager* shaderManager;
		std::map<std::string, Program*> mPrograms;
	};
}

#endif /* Shader_h */
