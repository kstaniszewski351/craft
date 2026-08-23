#pragma once

#include <bgfx/bgfx.h>
#include <string>
#include <filesystem>

class ShaderManager {
 public:
  ShaderManager(std::string path);
  bgfx::ProgramHandle LoadProgram(std::string vertex_path, std::string fragment_path);
 private:
  std::filesystem::path path_;
};