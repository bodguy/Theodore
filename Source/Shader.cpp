#include "Shader.h"
#include "Vector4d.h"
#include "Vector3d.h"
#include "Vector2d.h"
#include "Matrix4x4.h"
#include "Color.h"
#include "Debug.h"
#include "File.h"
#include <cstdlib>
#include <regex>
#include <sstream>

namespace Quark {
    ////////////////////////////////////////////////////////////////////////////////////
    // Shader
    
    Shader::Shader(const Enumeration::ShaderType type) : mIsCompiled(0) {
        mShaderID = glCreateShader(type);
    }
    
    Shader::~Shader() {
        glDeleteShader(mShaderID);
    }
    
    int Shader::Compile(const std::string& source) {
        Debug::Log("Compiling shader\n");
        const char *c_str = source.c_str();
        glShaderSource(mShaderID, 1, &c_str, NULL);
        glCompileShader(mShaderID);
        
        GLint result = GL_FALSE;
        int length;
        
        glGetShaderiv(mShaderID, GL_COMPILE_STATUS, &result);
        glGetShaderiv(mShaderID, GL_INFO_LOG_LENGTH, &length);
        if (length > 0) {
            GLchar* message = (GLchar*)malloc(sizeof(char)* length + 1);
            glGetShaderInfoLog(mShaderID, length, NULL, message);
            Debug::Log("%s\n", message);
            free(message);
        }
        
        return mIsCompiled = result;
    }
    
    int Shader::IsCompiled() const {
        return mIsCompiled;
    }
    
    std::string Shader::PreprocessIncludes(const std::string& source, int level) {
        // https://www.opengl.org/discussion_boards/showthread.php/169209-include-in-glsl
        std::stringstream output;
        static const std::regex re("^[ ]*#[ ]*include[ ]+[\"<](.*)[\">].*");
        std::smatch matches;
        
        try {
            if (std::regex_search(source, matches, re)) {
                std::string include_file = matches.str(1);
                std::string include_string;
                
                File file(include_file, Enumeration::Read);
                if (!file.IsOpen()) {
                    Debug::Log("%s : fatal error: cannot open include file\n", include_file.c_str());
                    return std::string();
                }
                include_string = file.ReadUntilEnd();
                output << PreprocessIncludes(include_string, level + 1) << std::endl;
            } else {
                output << source << std::endl;
            }
        }
        catch (std::regex_error& e) {
            // Syntax error in the regular expression
            Debug::Log("%d", e.what());
        }
        
        if (level == 0) {
            output << std::regex_replace(source, re, " ") << std::endl;
        }
        
        return output.str();
    }
    
    ////////////////////////////////////////////////////////////////////////////////////
    // Program
    
    Program::Program() {
        mProgramID = glCreateProgram();
    }
    
    Program::Program(const Shader& vertex) {
        mProgramID = glCreateProgram();
        AttachShader(vertex);
        Link();
    }
    
    Program::Program(const Shader& vertex, const Shader& fragment) {
        mProgramID = glCreateProgram();
        AttachShader(vertex);
        AttachShader(fragment);
        Link();
    }
    
    Program::Program(const Shader& vertex, const Shader& fragment, const Shader& geometry) {
        mProgramID = glCreateProgram();
        AttachShader(vertex);
        AttachShader(fragment);
        AttachShader(geometry);
        Link();
    }

	Program::Program(const Shader& vertex, const Shader& fragment, const Shader& geometry, const Shader& tessControl, const Shader& tessEval) {
		mProgramID = glCreateProgram();
		AttachShader(vertex);
		AttachShader(fragment);
		AttachShader(geometry);
		AttachShader(tessControl);
		AttachShader(tessEval);
		Link();
	}
    
    Program::~Program() {
        glDeleteProgram(mProgramID);
    }
    
    void Program::AttachShader(const Shader& shader) {
        glAttachShader(mProgramID, shader.mShaderID);
    }
    
    void Program::DetachShader(const Shader& shader) {
        glDetachShader(mProgramID, shader.mShaderID);
    }
    
    int Program::Link() {
        Debug::Log("Linking program\n");
        glLinkProgram(mProgramID);
        
        GLint result = GL_FALSE;
        int length;
        
        glGetProgramiv(mProgramID, GL_LINK_STATUS, &result);
        glGetProgramiv(mProgramID, GL_INFO_LOG_LENGTH, &length);
        if (length > 0) {
            GLchar* message = (GLchar*)malloc(sizeof(char)* length + 1);
            glGetProgramInfoLog(mProgramID, length, NULL, message);
            Debug::Log("%s\n", message);
            free(message);
        }
        
        return result;
    }
    
    void Program::Use() {
        glUseProgram(mProgramID);
    }

	void Program::UnUse() {
		glUseProgram(NULL);
	}
    
    unsigned int Program::GetProgramID() const {
        return mProgramID;
    }
    
    Attribute Program::GetAttribute(const std::string& name) {
        return glGetAttribLocation(mProgramID, name.c_str());
    }
    
    Uniform Program::GetUniform(const std::string& name) {
        return glGetUniformLocation(mProgramID, name.c_str());
    }
    
    Uniform Program::GetUniformBlockIndex(const std::string& name) {
        return glGetUniformBlockIndex(mProgramID, name.c_str());
    }
    
    void Program::SetUniform(const Uniform& uniform, int value) {
        glUniform1i(uniform, value);
    }
    
    void Program::SetUniform(const Uniform& uniform, unsigned int value) {
        glUniform1i(uniform, value);
    }
    
    void Program::SetUniform(const Uniform& uniform, float value) {
        glUniform1f(uniform, value);
    }
    
    void Program::SetUniform(const Uniform& uniform, const Vector2d& value) {
        glUniform2f(uniform, value.x, value.y);
    }
    
    void Program::SetUniform(const Uniform& uniform, const Vector3d& value) {
        glUniform3f(uniform, value.x, value.y, value.z);
    }
    
    void Program::SetUniform(const Uniform& uniform, const Vector4d& value) {
        glUniform4f(uniform, value.x, value.y, value.z, value.w);
    }
    
    void Program::SetUniform(const Uniform& uniform, const Color& value) {
        glUniform4f(uniform, value.r, value.g, value.b, value.a);
    }
    
    void Program::SetUniform(const Uniform& uniform, const float* values, int count) {
        glUniform1fv(uniform, count, values);
    }
    
    void Program::SetUniform(const Uniform& uniform, const Vector2d* values, int count) {
        glUniform2fv(uniform, count, (float*)values);
    }
    
    void Program::SetUniform(const Uniform& uniform, const Vector3d* values, int count) {
        glUniform3fv(uniform, count, (float*)values);
    }
    
    void Program::SetUniform(const Uniform& uniform, const Vector4d* values, int count) {
        glUniform4fv(uniform, count, (float*)values);
    }
    
    void Program::SetUniform(const Uniform& uniform, const Color* values, int count) {
        glUniform4fv(uniform, count, (float*)values);
    }
    
    void Program::SetUniform(const Uniform& uniform, const Matrix4x4& value) {
        glUniformMatrix4fv(uniform, 1, GL_FALSE, value.Pointer());
    }
    
    void Program::SetUniformBlock(const Uniform& uniform, const unsigned int bindingPoint) {
        glUniformBlockBinding(mProgramID, uniform, bindingPoint);
    }
}
