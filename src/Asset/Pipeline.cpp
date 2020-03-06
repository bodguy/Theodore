// Copyright (C) 2017 by bodguy
// This code is licensed under Apache 2.0 license (see LICENSE.md for details)

#include "Pipeline.h"
#include "Helper/Debug.h"
#include "Core/ShaderManager.h"
#include "Math/Color.h"
#include "Math/Matrix4x4.h"
#include "Math/Vector2d.h"
#include "Math/Vector3d.h"
#include "Math/Vector4d.h"

namespace Theodore {
	Pipeline::Pipeline(const std::string& name) {
		pipelineId = glCreateProgram();
		this->name = name;
	}

	Pipeline::Pipeline(const std::string& name, const Shader& vertex) {
		pipelineId = glCreateProgram();
		this->name = name;
		AttachShader(vertex);
		Link();
	}

	Pipeline::Pipeline(const std::string& name, const Shader& vertex, const Shader& fragment) {
		pipelineId = glCreateProgram();
		this->name = name;
		AttachShader(vertex);
		AttachShader(fragment);
		Link();
	}

	Pipeline::Pipeline(const std::string& name, const Shader& vertex, const Shader& fragment, const Shader& geometry) {
		pipelineId = glCreateProgram();
		this->name = name;
		AttachShader(vertex);
		AttachShader(fragment);
		AttachShader(geometry);
		Link();
	}

	Pipeline::Pipeline(const std::string& name, const Shader& vertex, const Shader& fragment, const Shader& geometry, const Shader& tessControl, const Shader& tessEval) {
		pipelineId = glCreateProgram();
		this->name = name;
		AttachShader(vertex);
		AttachShader(fragment);
		AttachShader(geometry);
		AttachShader(tessControl);
		AttachShader(tessEval);
		Link();
	}

	Pipeline::~Pipeline() { glDeleteProgram(pipelineId); }

	void Pipeline::AttachShader(const Shader& shader) { glAttachShader(pipelineId, shader.shaderId); }

	void Pipeline::DetachShader(const Shader& shader) { glDetachShader(pipelineId, shader.shaderId); }

	int Pipeline::Link() {
		Debug::Log("[PipelineID: %d] '%s' Link program", pipelineId, name.c_str());
		glLinkProgram(pipelineId);

		int result = false;
		int length;

		glGetProgramiv(pipelineId, GL_LINK_STATUS, &result);
		glGetProgramiv(pipelineId, GL_INFO_LOG_LENGTH, &length);
		if (length > 0) {
			char* message = (char*)malloc(sizeof(char) * length + 1);
			glGetProgramInfoLog(pipelineId, length, NULL, message);
			Debug::Error("%s", message);
			free(message);
			return false;
		}

		if (result) {
			if (!ShaderManager::Append(this)) {
				Debug::Warn("Pipeline [%s] is already stored", name.c_str());
				glDeleteProgram(pipelineId);
				return false;
			}
		}

		return result;
	}

	void Pipeline::Use() { glUseProgram(pipelineId); }

	void Pipeline::UnUse() { glUseProgram(NULL); }

	unsigned int Pipeline::GetPipelineID() const { return pipelineId; }

	std::string Pipeline::GetName() const {
		return name;
	}

	Attribute Pipeline::GetAttribute(const std::string& name) { return glGetAttribLocation(pipelineId, name.c_str()); }

	Uniform Pipeline::GetUniform(const std::string& name) { return glGetUniformLocation(pipelineId, name.c_str()); }

	Uniform Pipeline::GetUniformBlockIndex(const std::string& name) { return glGetUniformBlockIndex(pipelineId, name.c_str()); }

	void Pipeline::SetUniform(const Uniform& uniform, int value) { glUniform1i(uniform, value); }

	void Pipeline::SetUniform(const Uniform& uniform, unsigned int value) { glUniform1i(uniform, value); }

	void Pipeline::SetUniform(const Uniform& uniform, float value) { glUniform1f(uniform, value); }

	void Pipeline::SetUniform(const Uniform& uniform, const Vector2d& value) { glUniform2f(uniform, value.x, value.y); }

	void Pipeline::SetUniform(const Uniform& uniform, const Vector3d& value) { glUniform3f(uniform, value.x, value.y, value.z); }

	void Pipeline::SetUniform(const Uniform& uniform, const Vector4d& value) { glUniform4f(uniform, value.x, value.y, value.z, value.w); }

	void Pipeline::SetUniform(const Uniform& uniform, const Color& value) { glUniform4f(uniform, value.r, value.g, value.b, value.a); }

	void Pipeline::SetUniform(const Uniform& uniform, const float* values, int count) { glUniform1fv(uniform, count, values); }

	void Pipeline::SetUniform(const Uniform& uniform, const Vector2d* values, int count) { glUniform2fv(uniform, count, (float*)values); }

	void Pipeline::SetUniform(const Uniform& uniform, const Vector3d* values, int count) { glUniform3fv(uniform, count, (float*)values); }

	void Pipeline::SetUniform(const Uniform& uniform, const Vector4d* values, int count) { glUniform4fv(uniform, count, (float*)values); }

	void Pipeline::SetUniform(const Uniform& uniform, const Color* values, int count) { glUniform4fv(uniform, count, (float*)values); }

	void Pipeline::SetUniform(const Uniform& uniform, const Matrix4x4& value) { glUniformMatrix4fv(uniform, 1, GL_FALSE, value.Pointer()); }

	void Pipeline::SetUniform(const Uniform& uniform, bool value) { glUniform1i(uniform, value); }

	void Pipeline::SetUniformBlock(const Uniform& uniform, const unsigned int bindingPoint) { glUniformBlockBinding(pipelineId, uniform, bindingPoint); }

	void Pipeline::SetUniform(const std::string& name, int value) { glUniform1i(GetUniform(name), value); }

	void Pipeline::SetUniform(const std::string& name, unsigned int value) { glUniform1i(GetUniform(name), value); }

	void Pipeline::SetUniform(const std::string& name, float value) { glUniform1f(GetUniform(name), value); }

	void Pipeline::SetUniform(const std::string& name, const Vector2d& value) { glUniform2f(GetUniform(name), value.x, value.y); }

	void Pipeline::SetUniform(const std::string& name, const Vector3d& value) { glUniform3f(GetUniform(name), value.x, value.y, value.z); }

	void Pipeline::SetUniform(const std::string& name, const Vector4d& value) { glUniform4f(GetUniform(name), value.x, value.y, value.z, value.w); }

	void Pipeline::SetUniform(const std::string& name, const Color& value) { glUniform4f(GetUniform(name), value.r, value.g, value.b, value.a); }

	void Pipeline::SetUniform(const std::string& name, const float* values, int count) { glUniform1fv(GetUniform(name), count, values); }

	void Pipeline::SetUniform(const std::string& name, const Vector2d* values, int count) { glUniform2fv(GetUniform(name), count, (float*)values); }

	void Pipeline::SetUniform(const std::string& name, const Vector3d* values, int count) { glUniform3fv(GetUniform(name), count, (float*)values); }

	void Pipeline::SetUniform(const std::string& name, const Vector4d* values, int count) { glUniform4fv(GetUniform(name), count, (float*)values); }

	void Pipeline::SetUniform(const std::string& name, const Color* values, int count) { glUniform4fv(GetUniform(name), count, (float*)values); }

	void Pipeline::SetUniform(const std::string& name, const Matrix4x4& value) { glUniformMatrix4fv(GetUniform(name), 1, GL_FALSE, value.Pointer()); }

	void Pipeline::SetUniform(const std::string& name, bool value) { glUniform1i(GetUniform(name), value); }

	void Pipeline::SetUniformBlock(const std::string& name, const unsigned int bindingPoint) { glUniformBlockBinding(pipelineId, GetUniform(name), bindingPoint); }

	void Pipeline::DispatchCompute(unsigned int x, unsigned int y, unsigned int z) { glDispatchCompute(x, y, z); }
}