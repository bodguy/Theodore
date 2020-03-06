// Copyright (C) 2017 by bodguy
// This code is licensed under Apache 2.0 license (see LICENSE.md for details)

#include "Shader.h"
#include <cstdlib>
#include <cstring>

#include "Helper/Debug.h"
#include "Helper/StringUtil.h"
#include "Helper/File.h"
#include "Math/Color.h"
#include "Math/Matrix4x4.h"
#include "Core/Application.h"
#include "Core/ShaderManager.h"

namespace Theodore {
  Shader::Shader(ShaderType type) {
    // TODO: BinaryShaderType is not implemented yet. Future consideration
    assetType = AssetType::TextShaderType;
		shaderId = glCreateShader(static_cast<GLenum>(type));
  }

  Shader::~Shader() { glDeleteShader(shaderId); }

  int Shader::Compile(const std::string& source) {
    const char* c_str = PreprocessFromString(const_cast<char*>(source.c_str()), Application::GetShaderPath());
    glShaderSource(shaderId, 1, &c_str, NULL);
    glCompileShader(shaderId);

    GLint compile_result = GL_FALSE;
    int log_length;
    glGetShaderiv(shaderId, GL_COMPILE_STATUS, &compile_result);
    glGetShaderiv(shaderId, GL_INFO_LOG_LENGTH, &log_length);
    if (log_length > 0) {
      std::vector<char> log_msg_arr(log_length);
      glGetShaderInfoLog(shaderId, log_length, NULL, log_msg_arr.data());
      std::string log_msg(std::begin(log_msg_arr), std::end(log_msg_arr));
      log_msg.erase(std::remove(log_msg.begin(), log_msg.end(), '\n'), log_msg.end());
      Debug::Error("%s", log_msg.substr(7).c_str());
    }

    return compile_result;
  }

  Pipeline* Shader::Find(const std::string& name) { return ShaderManager::shaderManager->loadedPiplineLookUpTable.find(name)->second; }

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
}  // namespace Theodore
