#include "shader_manager.h"
#include "file.h"
#include <bgfx/bgfx.h>
#include <string>
#include <filesystem>

ShaderManager::ShaderManager(std::string path) {
  std::string shader_path;

  bgfx::RendererType::Enum renderer = bgfx::getRendererType();
  switch(renderer) {
    case bgfx::RendererType::OpenGL: shader_path = "glsl"; break;
    case bgfx::RendererType::Vulkan: shader_path = "spirv"; break;
    case bgfx::RendererType::Metal: shader_path = "metal"; break;

    default: throw std::runtime_error("Cannot get shader path, unsupported renderer"); break;
  }

  path_ = std::filesystem::path(path) / shader_path;
}

bgfx::ShaderHandle LoadShader(std::string path) {
  std::vector<char> data = ReadFile(path);
  return bgfx::createShader(bgfx::copy(data.data(), data.size()));
}

bgfx::ProgramHandle ShaderManager::LoadProgram(std::string vertex_path, std::string fragment_path) {


  bgfx::ShaderHandle vs = LoadShader(path_ / vertex_path.append(".bin"));
  bgfx::ShaderHandle fs = LoadShader(path_ / fragment_path.append(".bin"));

  bgfx::ProgramHandle program = bgfx::createProgram(vs, fs, true);
  return program;
}