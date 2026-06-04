#pragma once

#include "asset.h"
#include <memory>
#include <string>
#include "asset_meta.h"
#include <glad/gl.h>
#include <glm/glm.hpp>


struct ShaderMeta : public AssetMeta {
  std::shared_ptr<Asset> Load() override;
  std::string fragment_path;
  std::string vertex_path;
};

class Shader : public Asset {
 public:
  Shader(const std::string& fragment_path, const std::string& vertex_path);
  void Use() const;
  void Reload();
  void BindUniformBlock(int index, int binding) const;
  ~Shader() override;
  template<typename T>
  void SetUniform(int location, T& value);
 private:
  std::string fragment_path_;
  std::string vertex_path_;
  GLuint id_;
};