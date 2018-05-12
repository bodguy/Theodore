#include "Shader.h"
#include "Vector4d.h"
#include "Vector3d.h"
#include "Vector2d.h"
#include "Matrix4x4.h"
#include "Color.h"
#include "Debug.h"
#include "File.h"
#include "Utility.h"
#include <cstdlib>
#include <regex>
#include <sstream>

namespace Quark {
    ////////////////////////////////////////////////////////////////////////////////////
    // Shader
    
    Shader::Shader(const ShaderType type) : mIsCompiled(0) {
		mType = AssetType::TextShaderType; // BinaryShaderType is not implemented yet. Future consideration
        mShaderID = glCreateShader(static_cast<GLenum>(type));
    }
    
    Shader::~Shader() {
        glDeleteShader(mShaderID);
    }
    
    int Shader::Compile(const std::string& source) {
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
            Debug::Log("%s", message);
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
                
                File file(include_file, OpenMode::Read);
                if (!file.IsOpen()) {
                    Debug::Log("%s : fatal error: cannot open include file", include_file.c_str());
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

	Program* Shader::Find(const std::string& name) {
		return ShaderManager::shaderManager->mPrograms.find(name)->second;
	}
    
    ////////////////////////////////////////////////////////////////////////////////////
    // Program
    
    Program::Program(const std::string& name) {
        mProgramID = glCreateProgram();
		mName = name;
    }
    
    Program::Program(const std::string& name, const Shader& vertex) {
        mProgramID = glCreateProgram();
		mName = name;
        AttachShader(vertex);
        Link();
    }
    
    Program::Program(const std::string& name, const Shader& vertex, const Shader& fragment) {
        mProgramID = glCreateProgram();
		mName = name;
        AttachShader(vertex);
        AttachShader(fragment);
        Link();
    }
    
    Program::Program(const std::string& name, const Shader& vertex, const Shader& fragment, const Shader& geometry) {
        mProgramID = glCreateProgram();
		mName = name;
        AttachShader(vertex);
        AttachShader(fragment);
        AttachShader(geometry);
        Link();
    }

	Program::Program(const std::string& name, const Shader& vertex, const Shader& fragment, const Shader& geometry, const Shader& tessControl, const Shader& tessEval) {
		mProgramID = glCreateProgram();
		mName = name;
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
        Debug::Log("Linking program...");
        glLinkProgram(mProgramID);
        
        GLint result = GL_FALSE;
        int length;
        
        glGetProgramiv(mProgramID, GL_LINK_STATUS, &result);
        glGetProgramiv(mProgramID, GL_INFO_LOG_LENGTH, &length);
        if (length > 0) {
            GLchar* message = (GLchar*)malloc(sizeof(char)* length + 1);
            glGetProgramInfoLog(mProgramID, length, NULL, message);
            Debug::Log("%s", message);
            free(message);
        }

		if (result) {
			if (!ShaderManager::Append(this)) {
				Debug::Log(mName + " is already managed in ShaderManager");
				glDeleteProgram(mProgramID);
				return GL_FALSE;
			}
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

	void Program::SetUniform(const Uniform& uniform, bool value) {
		glUniform1i(uniform, value);
	}
    
    void Program::SetUniformBlock(const Uniform& uniform, const unsigned int bindingPoint) {
        glUniformBlockBinding(mProgramID, uniform, bindingPoint);
    }

	// ===

	void Program::SetUniform(const std::string& name, int value) {
		glUniform1i(GetUniform(name), value);
	}

	void Program::SetUniform(const std::string& name, unsigned int value) {
		glUniform1i(GetUniform(name), value);
	}

	void Program::SetUniform(const std::string& name, float value) {
		glUniform1f(GetUniform(name), value);
	}

	void Program::SetUniform(const std::string& name, const Vector2d& value) {
		glUniform2f(GetUniform(name), value.x, value.y);
	}

	void Program::SetUniform(const std::string& name, const Vector3d& value) {
		glUniform3f(GetUniform(name), value.x, value.y, value.z);
	}

	void Program::SetUniform(const std::string& name, const Vector4d& value) {
		glUniform4f(GetUniform(name), value.x, value.y, value.z, value.w);
	}

	void Program::SetUniform(const std::string& name, const Color& value) {
		glUniform4f(GetUniform(name), value.r, value.g, value.b, value.a);
	}

	void Program::SetUniform(const std::string& name, const float* values, int count) {
		glUniform1fv(GetUniform(name), count, values);
	}

	void Program::SetUniform(const std::string& name, const Vector2d* values, int count) {
		glUniform2fv(GetUniform(name), count, (float*)values);
	}

	void Program::SetUniform(const std::string& name, const Vector3d* values, int count) {
		glUniform3fv(GetUniform(name), count, (float*)values);
	}

	void Program::SetUniform(const std::string& name, const Vector4d* values, int count) {
		glUniform4fv(GetUniform(name), count, (float*)values);
	}

	void Program::SetUniform(const std::string& name, const Color* values, int count) {
		glUniform4fv(GetUniform(name), count, (float*)values);
	}

	void Program::SetUniform(const std::string& name, const Matrix4x4& value) {
		glUniformMatrix4fv(GetUniform(name), 1, GL_FALSE, value.Pointer());
	}

	void Program::SetUniform(const std::string& name, bool value) {
		glUniform1i(GetUniform(name), value);
	}

	void Program::SetUniformBlock(const std::string& name, const unsigned int bindingPoint) {
		glUniformBlockBinding(mProgramID, GetUniform(name), bindingPoint);
	}
	// ===

	void Program::DispatchCompute(unsigned int x, unsigned int y, unsigned int z) {
		glDispatchCompute(x, y, z);
	}

	ShaderManager* ShaderManager::shaderManager = nullptr;
	ShaderManager::ShaderManager() {
		shaderManager = this;
		shaderManager->mPrograms.clear();
	}

	ShaderManager::~ShaderManager() {
		for (auto& i : mPrograms) {
			SafeDealloc(i.second);
		}
		mPrograms.clear();
	}

	bool ShaderManager::Append(Program* program) {
		std::pair<std::map<std::string, Program*>::iterator, bool> result;
		result = shaderManager->mPrograms.insert(std::pair<std::string, Program*>(program->mName, program));
		return result.second;
	}
}
