#pragma once

#include <string>
#include <glad/gl.h>
#include <glm/glm.hpp>

namespace GFX {
  class Shader {
   public:
    Shader(const std::string& fragment_path, const std::string& vertex_path);
    void Use() const;
    void Reload();
    void BindUniformBlock(int index, int binding) const;
    ~Shader();
    template<typename T>
    void SetUniform(int location, T& value);
   private:
    std::string fragment_path_;
    std::string vertex_path_;
    GLuint id_;
  };
}
