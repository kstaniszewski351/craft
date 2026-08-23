#include "file.h"

#include <fstream>
#include <ios>
#include <iosfwd>
#include <sstream>
#include <stdexcept>
#include <string>

std::string ReadText(const std::string& path) {
  std::ifstream file;
  file.open(path);

  if(!file) {
    throw std::runtime_error(std::string("Failed to open text file: ") + path);
  }

  std::ostringstream source_stream;
  source_stream << file.rdbuf();
  file.close();

  std::string source_string = source_stream.str();

  return source_string;
};

std::vector<char> ReadFile(std::string path) {
  std::ifstream file(path, std::ios::binary | std::ios::ate);

  if(!file.is_open()) {
    throw std::runtime_error(std::string("Failed to open binary file: ") + path);
  }

  file.seekg(0, std::ios::end);
  std::streampos size = file.tellg();
  file.seekg(0, std::ios::beg);

  std::vector<char> buffer(size);
  if(!file.read(&buffer[0], size)) {
    throw std::runtime_error(std::string("Failed to load binary file: " + path));
  }

  return buffer;
}