#include "shader.h"
#include "text_file.h"
#include <glm/fwd.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <stdexcept>
#include <string>
#include <vector>

namespace GFX {
  std::string ParseIncludes(const std::string& string) {
    std::string result;

    int last_insert = 0;

    for(int i = 0; i < string.length(); i++) {
      if(string[i] != '#') {
        continue;
      }

      int line_begin = i + 1;
      int line_end = string.find('\n', line_begin);

      int space_index = string.find(' ', line_begin);
      if(space_index == std::string::npos) {
        continue;
      }

      std::string word = string.substr(line_begin, space_index - (line_begin));

      if(word == "include") {
        int quotation_begin = string.find_first_not_of(' ', space_index);
        if(quotation_begin == std::string::npos || string[quotation_begin] != '"') {
          throw std::runtime_error("Shader include failed: invalid argument");
        }

        int quotation_end = string.find_first_of('"', quotation_begin + 1);
        if(quotation_end == std::string::npos) {
          throw std::runtime_error("Shader include failed: quotation doesnt close");
        }
        std::string path = string.substr(quotation_begin + 1, (quotation_end - 1) - quotation_begin);
        std::string source = ReadText(path);

        result.append(string, last_insert, i - last_insert);
        last_insert = line_end + 1; 
        result.append(ParseIncludes(source));
        result.append("\n");
      }
    }

    result.append(string, last_insert);
    
    return result;
  }

  GLuint LoadShader(const std::string& path, GLenum type) {
    std::string source_string = ParseIncludes(ReadText(path));

    const char* source = source_string.c_str();

    GLuint shader;
    shader = glCreateShader(type);
    glShaderSource(shader, 1, &source, NULL);
    glCompileShader(shader);

    GLint compile_success;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &compile_success);
    if(compile_success == GL_FALSE) {
      std::string error_log;

      std::string log = "Driver returned no log";
      GLsizei log_lenght;
      glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &log_lenght);
      if(log_lenght != 0) {
        std::vector<char> log_buffer = std::vector<char>(log_lenght);
        GLsizei returned_lenght;
        glGetShaderInfoLog(shader, log_lenght, &returned_lenght, log_buffer.data());

        log = std::string(log_buffer.data(), returned_lenght);
      }

      glDeleteShader(shader);
      throw std::runtime_error(std::string("Failed to compile shader: ") + path + "\nLog: \n" + log);
    }

    return shader;
  }

  void Shader::Reload() {
    GLuint fragment = LoadShader(fragment_path_, GL_FRAGMENT_SHADER);
    GLuint vertex = LoadShader(vertex_path_, GL_VERTEX_SHADER);

    id_ = glCreateProgram();
    glAttachShader(id_, vertex);
    glAttachShader(id_, fragment);
    glLinkProgram(id_);

    GLint link_status;
    glGetProgramiv(id_, GL_LINK_STATUS, &link_status);
    if(link_status == GL_FALSE) {
      glDeleteShader(fragment);
      glDeleteShader(vertex);
      throw std::runtime_error("Failed to link shader program");
    }
  }

  Shader::Shader(const std::string& fragment_path, const std::string& vertex_path) : fragment_path_(fragment_path), vertex_path_(vertex_path) {
    Reload();
  }

  void Shader::BindUniformBlock(int index, int binding) const {
    glUniformBlockBinding(id_, index, binding);
  }

  void Shader::Use() const {
    glUseProgram(id_);
  }

  Shader::~Shader() {
    glDeleteProgram(id_);
  }
  
  //float vectors
  template<>
  void Shader::SetUniform<float>(int location, float& value) {
    Use();
    glUniform1fv(location, 1, &value);
  }

  template<>
  void Shader::SetUniform<glm::vec2>(int location, glm::vec2& value) {
    Use();
    glUniform2fv(location, 1, glm::value_ptr(value));
  }

  template<>
  void Shader::SetUniform<glm::vec3>(int location, glm::vec3& value) {
    Use();
    glUniform3fv(location, 1, glm::value_ptr(value));
  }

  template<>
  void Shader::SetUniform<glm::vec4>(int location, glm::vec4& value) {
    Use();
    glUniform4fv(location, 1, glm::value_ptr(value));
  }

  //integer vectors
  template<>
  void Shader::SetUniform<int>(int location, int& value) {
    Use();
    glUniform1iv(location, 1, &value);
  }

  template<>
  void Shader::SetUniform<glm::ivec2>(int location, glm::ivec2& value) {
    Use();
    glUniform2iv(location, 1, glm::value_ptr(value));
  }

  template<>
  void Shader::SetUniform<glm::ivec3>(int location, glm::ivec3& value) {
    Use();
    glUniform3iv(location, 1, glm::value_ptr(value));
  }

  template<>
  void Shader::SetUniform<glm::ivec4>(int location, glm::ivec4& value) {
    Use();
    glUniform4iv(location, 1, glm::value_ptr(value));
  }

  //unsigned integer vectors
  template<>
  void Shader::SetUniform<unsigned int>(int location, unsigned int& value) {
    Use();
    glUniform1uiv(location, 1, &value);
  }

  template<>
  void Shader::SetUniform<glm::uvec2>(int location, glm::uvec2& value) {
    Use();
    glUniform2uiv(location, 1, glm::value_ptr(value));
  }

  template<>
  void Shader::SetUniform<glm::uvec3>(int location, glm::uvec3& value) {
    Use();
    glUniform3uiv(location, 1, glm::value_ptr(value));
  }

  template<>
  void Shader::SetUniform<glm::uvec4>(int location, glm::uvec4& value) {
    Use();
    glUniform4uiv(location, 1, glm::value_ptr(value));
  }

  //matrices
  template<>
  void Shader::SetUniform<glm::mat2>(int location, glm::mat2& value) {
    Use();
    glUniformMatrix2fv(location, 1, false, glm::value_ptr(value));
  }

  template<>
  void Shader::SetUniform<glm::mat3>(int location, glm::mat3& value) {
    Use();
    glUniformMatrix3fv(location, 1, false, glm::value_ptr(value));
  }

  template<>
  void Shader::SetUniform<glm::mat4>(int location, glm::mat4& value) {
    Use();
    glUniformMatrix4fv(location, 1, false, glm::value_ptr(value));
  }

  template<>
  void Shader::SetUniform<glm::mat2x3>(int location, glm::mat2x3& value) {
    Use();
    glUniformMatrix2x3fv(location, 1, false, glm::value_ptr(value));
  }

  template<>
  void Shader::SetUniform<glm::mat3x2>(int location, glm::mat3x2& value) {
    Use();
    glUniformMatrix3x2fv(location, 1, false, glm::value_ptr(value));
  }

  template<>
  void Shader::SetUniform<glm::mat2x4>(int location, glm::mat2x4& value) {
    Use();
    glUniformMatrix2x4fv(location, 1, false, glm::value_ptr(value));
  }

  template<>
  void Shader::SetUniform<glm::mat4x2>(int location, glm::mat4x2& value) {
    Use();
    glUniformMatrix4x2fv(location, 1, false, glm::value_ptr(value));
  }

  template<>
  void Shader::SetUniform<glm::mat3x4>(int location, glm::mat3x4& value) {
    Use();
    glUniformMatrix3x4fv(location, 1, false, glm::value_ptr(value));
  }

  template<>
  void Shader::SetUniform<glm::mat4x3>(int location, glm::mat4x3& value) {
    Use();
    glUniformMatrix4x3fv(location, 1, false, glm::value_ptr(value));
  }
}

