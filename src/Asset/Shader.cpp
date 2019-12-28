// Copyright (C) 2017 by bodguy
// This code is licensed under Apache 2.0 license (see LICENSE.md for details)

#include "Shader.h"

#include <algorithm>
#include <cstdlib>
#include <cstring>
#include <vector>

#include "Helper/Debug.h"
#include "Helper/StringUtil.h"
#include "Helper/Utility.h"
#include "Math/Color.h"
#include "Math/Matrix4x4.h"
#include "Math/Vector2d.h"
#include "Math/Vector3d.h"
#include "Math/Vector4d.h"
#include "Object/Application.h"

namespace Theodore {
  ////////////////////////////////////////////////////////////////////////////////////
  // Shader

  Shader::Shader(ShaderType type) {
    // BinaryShaderType is not implemented yet. Future consideration
    mType = AssetType::TextShaderType;
    mShaderID = glCreateShader(static_cast<GLenum>(type));
  }

  Shader::~Shader() { glDeleteShader(mShaderID); }

  int Shader::Compile(const std::string& source) {
    const char* c_str = PreprocessFromString(const_cast<char*>(source.c_str()), Application::GetShaderPath());
    glShaderSource(mShaderID, 1, &c_str, NULL);
    glCompileShader(mShaderID);

    GLint compile_result = GL_FALSE;
    int log_length;
    glGetShaderiv(mShaderID, GL_COMPILE_STATUS, &compile_result);
    glGetShaderiv(mShaderID, GL_INFO_LOG_LENGTH, &log_length);
    if (log_length > 0) {
      std::vector<char> log_msg_arr(log_length);
      glGetShaderInfoLog(mShaderID, log_length, NULL, log_msg_arr.data());
      std::string log_msg(std::begin(log_msg_arr), std::end(log_msg_arr));
      log_msg.erase(std::remove(log_msg.begin(), log_msg.end(), '\n'), log_msg.end());
      Debug::Error("%s", log_msg.substr(7).c_str());
    }

    return compile_result;
  }

  Pipeline* Shader::Find(const std::string& name) { return ShaderManager::shaderManager->mPipelines.find(name)->second; }

  char* Shader::PreprocessFromString(char* str, const std::string& path) {
    std::vector<ShaderPreprocess*> plist;
    int num = RetrieveDirective(str, plist);
    size_t source_len = strlen(str);
    char* text = nullptr;
    size_t textlen = 0, last = 0;

    for (int i = 0; i < num; i++) {
      text = ReAlloc(text, &textlen, str + last, plist[i]->offset - last);
      if (plist[i]->filename != nullptr) {
        std::string filename = path + "/" + plist[i]->filename;
        char* inc = PreprocessFromFile(filename, path);
        if (inc == nullptr) {
          Free(plist);
          return nullptr;
        }
        text = ReAlloc(text, &textlen, inc, strlen(inc));
        free(inc);
      }
      last = plist[i]->end;
    }
    text = ReAlloc(text, &textlen, str + last, source_len - last + 1);
    Free(plist);
    return text;
  }

  char* Shader::PreprocessFromFile(const std::string& filename, const std::string& path) {
    char* result;
    char* text = Read(filename);
    if (text == nullptr) {
      Debug::Error("could not find '%s'", filename.c_str());
      return nullptr;
    }
    result = PreprocessFromString(text, path);
    free(text);
    return result;
  }

  int Shader::RetrieveDirective(char* text, std::vector<ShaderPreprocess*>& plist) {
    int line_count = 1;
    int inc_count = 0;
    char *s = text, *start;
    while (*s) {
      start = s;
      while (*s == ' ' || *s == '\t') ++s;
      if (*s == '#') {
        ++s;
        while (*s == ' ' || *s == '\t') ++s;
        if (0 == strncmp(s, "include", 7) && StringUtil::IsSpace(s[7])) {
          s += 7;
          while (*s == ' ' || *s == '\t') ++s;
          if (*s == '"') {
            char* t = ++s;
            while (*t != '"' && *t != '\n' && *t != '\r' && *t != 0) ++t;
            if (*t == '"') {
              char* filename = (char*)malloc(t - s + 1);
              memcpy(filename, s, t - s);
              filename[t - s] = 0;
              s = t;
              while (*s != '\r' && *s != '\n' && *s != 0) ++s;
              plist.emplace_back(new ShaderPreprocess(start - text, s - text, filename, line_count + 1));
              ++inc_count;
            }
          }
        }
      }
      while (*s != '\r' && *s != '\n' && *s != 0) ++s;
      if (*s == '\r' || *s == '\n') {
        s = s + (s[0] + s[1] == '\r' + '\n' ? 2 : 1);
      }
      ++line_count;
    }
    return inc_count;
  }

  char* Shader::Read(const std::string& filename) {
    File file(filename, OpenMode::ReadBinary);
    if (!file.IsOpen()) return nullptr;
    size_t len = file.GetSize();
    char* text = (char*)malloc(len + 1);
    if (text == nullptr) return nullptr;
    file.ReadBuf(text, 1, len);
    file.Close();
    text[len] = 0;
    return text;
  }

  void Shader::Free(const std::vector<ShaderPreprocess*>& list) {
    for (auto i : list) {
      free(i->filename);
      delete (i);
    }
  }

  char* Shader::ReAlloc(char* str, size_t* curlen, char* addstr, size_t addlen) {
    str = (char*)realloc(str, *curlen + addlen);
    memcpy(str + *curlen, addstr, addlen);
    *curlen += addlen;
    return str;
  }

  ////////////////////////////////////////////////////////////////////////////////////
  // Program

  Pipeline::Pipeline(const std::string& name) {
    mPipelineID = glCreateProgram();
    mName = name;
  }

  Pipeline::Pipeline(const std::string& name, const Shader& vertex) {
    mPipelineID = glCreateProgram();
    mName = name;
    AttachShader(vertex);
    Link();
  }

  Pipeline::Pipeline(const std::string& name, const Shader& vertex, const Shader& fragment) {
    mPipelineID = glCreateProgram();
    mName = name;
    AttachShader(vertex);
    AttachShader(fragment);
    Link();
  }

  Pipeline::Pipeline(const std::string& name, const Shader& vertex, const Shader& fragment, const Shader& geometry) {
    mPipelineID = glCreateProgram();
    mName = name;
    AttachShader(vertex);
    AttachShader(fragment);
    AttachShader(geometry);
    Link();
  }

  Pipeline::Pipeline(const std::string& name, const Shader& vertex, const Shader& fragment, const Shader& geometry, const Shader& tessControl, const Shader& tessEval) {
    mPipelineID = glCreateProgram();
    mName = name;
    AttachShader(vertex);
    AttachShader(fragment);
    AttachShader(geometry);
    AttachShader(tessControl);
    AttachShader(tessEval);
    Link();
  }

  Pipeline::~Pipeline() { glDeleteProgram(mPipelineID); }

  void Pipeline::AttachShader(const Shader& shader) { glAttachShader(mPipelineID, shader.mShaderID); }

  void Pipeline::DetachShader(const Shader& shader) { glDetachShader(mPipelineID, shader.mShaderID); }

  int Pipeline::Link() {
    Debug::Log("[PipelineID: %d] '%s' Link program", mPipelineID, mName.c_str());
    glLinkProgram(mPipelineID);

    GLint result = GL_FALSE;
    int length;

    glGetProgramiv(mPipelineID, GL_LINK_STATUS, &result);
    glGetProgramiv(mPipelineID, GL_INFO_LOG_LENGTH, &length);
    if (length > 0) {
      GLchar* message = (GLchar*)malloc(sizeof(char) * length + 1);
      glGetProgramInfoLog(mPipelineID, length, NULL, message);
      Debug::Error("%s", message);
      free(message);
      return GL_FALSE;
    }

    if (result) {
      if (!ShaderManager::Append(this)) {
        Debug::Warn("'%s' already managed", mName.c_str());
        glDeleteProgram(mPipelineID);
        return GL_FALSE;
      }
    }

    return result;
  }

  void Pipeline::Use() { glUseProgram(mPipelineID); }

  void Pipeline::UnUse() { glUseProgram(NULL); }

  unsigned int Pipeline::GetPipelineID() const { return mPipelineID; }

  Attribute Pipeline::GetAttribute(const std::string& name) { return glGetAttribLocation(mPipelineID, name.c_str()); }

  Uniform Pipeline::GetUniform(const std::string& name) { return glGetUniformLocation(mPipelineID, name.c_str()); }

  Uniform Pipeline::GetUniformBlockIndex(const std::string& name) { return glGetUniformBlockIndex(mPipelineID, name.c_str()); }

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

  void Pipeline::SetUniformBlock(const Uniform& uniform, const unsigned int bindingPoint) { glUniformBlockBinding(mPipelineID, uniform, bindingPoint); }

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

  void Pipeline::SetUniformBlock(const std::string& name, const unsigned int bindingPoint) { glUniformBlockBinding(mPipelineID, GetUniform(name), bindingPoint); }

  void Pipeline::DispatchCompute(unsigned int x, unsigned int y, unsigned int z) { glDispatchCompute(x, y, z); }

  ShaderManager* ShaderManager::shaderManager = nullptr;
  ShaderManager::ShaderManager() {
    shaderManager = this;
    shaderManager->mPipelines.clear();
  }

  ShaderManager::~ShaderManager() {
    for (auto& i : mPipelines) {
      SafeDealloc(i.second);
    }
    mPipelines.clear();
  }

  bool ShaderManager::Append(Pipeline* program) {
    std::pair<std::map<std::string, Pipeline*>::iterator, bool> result;
    result = shaderManager->mPipelines.insert(std::pair<std::string, Pipeline*>(program->mName, program));
    return result.second;
  }
}  // namespace Theodore
